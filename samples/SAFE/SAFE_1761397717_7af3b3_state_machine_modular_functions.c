//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} event_t;

static event_t classify_char(char c) {
    if (c >= '0' && c <= '9') return EVENT_DIGIT;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_LETTER;
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c == '\0' || c == '\n') return EVENT_TERMINATOR;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event) {
    switch (event) {
        case EVENT_DIGIT: return STATE_READING_NUMBER;
        case EVENT_LETTER: return STATE_READING_WORD;
        case EVENT_SPACE: return STATE_START;
        case EVENT_TERMINATOR: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event) {
    switch (event) {
        case EVENT_DIGIT: return STATE_READING_NUMBER;
        case EVENT_SPACE: return STATE_START;
        case EVENT_TERMINATOR: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event) {
    switch (event) {
        case EVENT_LETTER: return STATE_READING_WORD;
        case EVENT_SPACE: return STATE_START;
        case EVENT_TERMINATOR: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_error(event_t event) {
    return STATE_ERROR;
}

static state_t handle_end(event_t event) {
    return STATE_END;
}

static void process_input(const char* input) {
    state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = input[i];
        event_t event = classify_char(current_char);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event);
                if (event == EVENT_SPACE || event == EVENT_TERMINATOR) {
                    printf("Found number\n");
                }
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event);
                if (event == EVENT_SPACE || event == EVENT_TERMINATOR) {
                    printf("Found word\n");
                }
                break;
            case STATE_ERROR:
                current_state = handle_error(event);
                break;
            case STATE_END:
                current_state = handle_end(event);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Error: Invalid input at position %zu\n", i);
            return;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current_state != STATE_END && current_state != STATE_ERROR) {
        printf("Error: Unexpected end of input\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(input);
    
    return 0;
}