//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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

static event_t get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t transition(state_t current, event_t event) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_TERMINATOR: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_SPACE: return STATE_START;
                case EVENT_TERMINATOR: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_TERMINATOR: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_ERROR:
            return STATE_ERROR;
        case STATE_END:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
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
        event_t event = get_event(current_char);
        state_t next_state = transition(current_state, event);
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid input at position %zu\n", i);
            return;
        }
        
        if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
            printf("Found number\n");
        } else if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            printf("Found word\n");
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            printf("Processing complete\n");
            return;
        }
    }
}

static int read_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }
    
    printf("Enter text to analyze (max %zu chars): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    
    if (read_input(input_buffer, sizeof(input_buffer)) == 0) {
        if (strlen(input_buffer) == 0) {
            printf("No input provided\n");
            return EXIT_FAILURE;
        }
        
        process_input(input_buffer);
        return EXIT_SUCCESS;
    } else {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
}