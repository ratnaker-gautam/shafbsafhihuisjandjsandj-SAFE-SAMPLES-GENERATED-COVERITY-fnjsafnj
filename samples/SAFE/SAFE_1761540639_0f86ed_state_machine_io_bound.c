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
    STATE_DONE
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (event == EVENT_LETTER) return STATE_READING_WORD;
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_DONE;
            return STATE_ERROR;
        case STATE_READING_NUMBER:
            if (event == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_DONE;
            return STATE_ERROR;
        case STATE_READING_WORD:
            if (event == EVENT_LETTER) return STATE_READING_WORD;
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_DONE;
            return STATE_ERROR;
        case STATE_ERROR:
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_DONE;
            return STATE_ERROR;
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

void process_input(const char* input) {
    State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    char current_token[MAX_INPUT_LEN] = {0};
    int token_len = 0;
    
    while (current_state != STATE_DONE && pos < MAX_INPUT_LEN) {
        char c = input[pos];
        Event event = get_event(c);
        State next_state = transition(current_state, event);
        
        if (current_state == STATE_READING_NUMBER && next_state != STATE_READING_NUMBER) {
            if (token_len > 0) {
                number_count++;
                current_token[token_len] = '\0';
                printf("Found number: %s\n", current_token);
                token_len = 0;
            }
        } else if (current_state == STATE_READING_WORD && next_state != STATE_READING_WORD) {
            if (token_len > 0) {
                word_count++;
                current_token[token_len] = '\0';
                printf("Found word: %s\n", current_token);
                token_len = 0;
            }
        }
        
        if (next_state == STATE_READING_NUMBER || next_state == STATE_READING_WORD) {
            if (token_len < MAX_INPUT_LEN - 1) {
                current_token[token_len++] = c;
            }
        }
        
        current_state = next_state;
        pos++;
        
        if (event == EVENT_EOF) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format at position %d\n", pos);
    } else {
        printf("Summary: %d numbers, %d words\n", number_count, word_count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}