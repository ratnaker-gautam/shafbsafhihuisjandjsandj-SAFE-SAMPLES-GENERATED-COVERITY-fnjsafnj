//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
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

State transition(State current, Event evt) {
    switch (current) {
        case STATE_START:
            if (evt == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (evt == EVENT_LETTER) return STATE_READING_WORD;
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_READING_NUMBER:
            if (evt == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_READING_WORD:
            if (evt == EVENT_LETTER) return STATE_READING_WORD;
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_ERROR:
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

void process_token(State state, const char* token, int len) {
    if (len <= 0) return;
    
    switch (state) {
        case STATE_READING_NUMBER:
            printf("Found number: %.*s\n", len, token);
            break;
        case STATE_READING_WORD:
            printf("Found word: %.*s\n", len, token);
            break;
        case STATE_ERROR:
            printf("Invalid token: %.*s\n", len, token);
            break;
        default:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char token[MAX_INPUT_LEN + 1];
    int token_len = 0;
    State current_state = STATE_START;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        Event evt = get_event(c);
        State next_state = transition(current_state, evt);
        
        if (current_state != next_state) {
            if (current_state == STATE_READING_NUMBER || 
                current_state == STATE_READING_WORD ||
                current_state == STATE_ERROR) {
                process_token(current_state, token, token_len);
                token_len = 0;
            }
        }
        
        if (next_state == STATE_READING_NUMBER || 
            next_state == STATE_READING_WORD ||
            next_state == STATE_ERROR) {
            if (token_len < MAX_INPUT_LEN) {
                token[token_len++] = c;
            }
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Input contains invalid characters\n");
        return 1;
    }
    
    return 0;
}