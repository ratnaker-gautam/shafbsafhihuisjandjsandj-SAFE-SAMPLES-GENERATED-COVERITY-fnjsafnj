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

void process_token(char *start, char *end, State state) {
    if (end <= start) return;
    
    size_t len = end - start;
    if (len >= MAX_INPUT_LEN) return;
    
    char token[MAX_INPUT_LEN];
    for (size_t i = 0; i < len && i < MAX_INPUT_LEN - 1; i++) {
        token[i] = *(start + i);
    }
    token[len] = '\0';
    
    if (state == STATE_READING_NUMBER) {
        printf("Number: %s\n", token);
    } else if (state == STATE_READING_WORD) {
        printf("Word: %s\n", token);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return EXIT_SUCCESS;
    }
    
    State current_state = STATE_START;
    char *token_start = input;
    char *current_pos = input;
    
    while (*current_pos != '\0' && current_state != STATE_END && current_state != STATE_ERROR) {
        Event ev = get_event(*current_pos);
        State next_state = transition(current_state, ev);
        
        if (current_state == STATE_START && (next_state == STATE_READING_NUMBER || next_state == STATE_READING_WORD)) {
            token_start = current_pos;
        } else if ((current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) && next_state == STATE_START) {
            process_token(token_start, current_pos, current_state);
        } else if (current_state == STATE_READING_NUMBER && next_state == STATE_ERROR) {
            printf("Error: invalid character in number\n");
        } else if (current_state == STATE_READING_WORD && next_state == STATE_ERROR) {
            printf("Error: invalid character in word\n");
        }
        
        current_state = next_state;
        current_pos++;
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        process_token(token_start, current_pos, current_state);
    }
    
    return EXIT_SUCCESS;
}