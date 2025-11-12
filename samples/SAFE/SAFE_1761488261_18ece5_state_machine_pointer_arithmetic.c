//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event) {
    static State transitions[][5] = {
        {STATE_READING_NUMBER, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_READING_NUMBER, STATE_ERROR, STATE_START, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };
    
    if (current >= MAX_STATES || event > EVENT_INVALID) {
        return STATE_ERROR;
    }
    
    return transitions[current][event];
}

void process_token(char *start, char *end, State token_type) {
    if (start >= end || !start || !end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT_LEN) return;
    
    char buffer[MAX_INPUT_LEN];
    char *buf_ptr = buffer;
    char *input_ptr = start;
    
    while (input_ptr < end && buf_ptr < buffer + MAX_INPUT_LEN - 1) {
        *buf_ptr = *input_ptr;
        buf_ptr++;
        input_ptr++;
    }
    *buf_ptr = '\0';
    
    if (token_type == STATE_READING_NUMBER) {
        printf("Found number: %s\n", buffer);
    } else if (token_type == STATE_READING_WORD) {
        printf("Found word: %s\n", buffer);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (!fgets(input, sizeof(input), stdin)) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_START;
    char *input_ptr = input;
    char *token_start = NULL;
    
    while (*input_ptr && current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_event(*input_ptr);
        State next_state = transition(current_state, event);
        
        if (current_state == STATE_START && (next_state == STATE_READING_NUMBER || next_state == STATE_READING_WORD)) {
            token_start = input_ptr;
        }
        
        if ((current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) && 
            next_state == STATE_START) {
            process_token(token_start, input_ptr, current_state);
            token_start = NULL;
        }
        
        if (next_state == STATE_ERROR) {
            printf("Invalid input format at position %ld\n", (long)(input_ptr - input));
            break;
        }
        
        current_state = next_state;
        input_ptr++;
        
        if (current_state == STATE_END) {
            if (token_start) {
                process_token(token_start, input_ptr - 1, current_state == STATE_READING_NUMBER ? STATE_READING_NUMBER : STATE_READING_WORD);
            }
            break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        process_token(token_start, input_ptr, current_state);
    }
    
    return EXIT_SUCCESS;
}