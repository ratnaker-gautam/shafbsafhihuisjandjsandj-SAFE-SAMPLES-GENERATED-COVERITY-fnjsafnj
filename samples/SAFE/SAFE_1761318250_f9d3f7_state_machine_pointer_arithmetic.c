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
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event ev) {
    static const State transitions[][5] = {
        {STATE_READING_NUMBER, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_READING_NUMBER, STATE_ERROR, STATE_START, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
        {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
    };
    
    if (current >= MAX_STATES || ev > EVENT_INVALID) return STATE_ERROR;
    return transitions[current][ev];
}

void process_token(char *start, char *end, State token_type) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT_LEN) return;
    
    char buffer[MAX_INPUT_LEN];
    for (size_t i = 0; i < len && i < MAX_INPUT_LEN - 1; i++) {
        buffer[i] = *(start + i);
    }
    buffer[len] = '\0';
    
    if (token_type == STATE_READING_NUMBER) {
        printf("Found number: %s\n", buffer);
    } else if (token_type == STATE_READING_WORD) {
        printf("Found word: %s\n", buffer);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_SUCCESS;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char *token_start = input;
    char *current_pos = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event ev = get_event(*current_pos);
        previous_state = current_state;
        current_state = transition(current_state, ev);
        
        if (previous_state != current_state) {
            if (previous_state == STATE_READING_NUMBER || previous_state == STATE_READING_WORD) {
                process_token(token_start, current_pos, previous_state);
            }
            
            if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
                token_start = current_pos;
            }
        }
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input format.\n");
            break;
        }
        
        if (*current_pos != '\0') {
            current_pos++;
        } else {
            break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        process_token(token_start, current_pos, current_state);
    }
    
    return EXIT_SUCCESS;
}