//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    unsigned int word_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->word_count = 0;
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t';
}

enum State handle_idle(struct StateMachine *sm, char input) {
    if (sm == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (isalpha(input)) {
            sm->buffer[sm->buffer_pos++] = input;
            return STATE_READING;
        }
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum State handle_reading(struct StateMachine *sm, char input) {
    if (sm == NULL) return STATE_ERROR;
    
    if (is_valid_char(input)) {
        if (isalpha(input)) {
            if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos++] = input;
            }
            return STATE_READING;
        } else if (input == ' ' || input == '\n' || input == '\t') {
            if (sm->buffer_pos > 0) {
                sm->buffer[sm->buffer_pos] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_IDLE;
        }
    }
    return STATE_ERROR;
}

enum State handle_processing(struct StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
    
    if (sm->buffer_pos > 0) {
        sm->word_count++;
        printf("Word %u: %s\n", sm->word_count, sm->buffer);
    }
    
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return STATE_IDLE;
}

void process_input(struct StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        switch (sm->current_state) {
            case STATE_IDLE:
                sm->current_state = handle_idle(sm, c);
                break;
            case STATE_READING:
                sm->current_state = handle_reading(sm, c);
                break;
            case STATE_PROCESSING:
                sm->current_state = handle_processing(sm);
                if (is_valid_char(c)) {
                    sm->current_state = handle_idle(sm, c);
                } else {
                    sm->current_state = STATE_ERROR;
                }
                break;
            case STATE_COMPLETE:
            case STATE_ERROR:
                return;
        }
        
        if (sm->current_state == STATE_ERROR) {
            printf("Error: Invalid input character '%c' at position %zu\n", c, i + 1);
            return;
        }
    }
    
    if (sm->current_state == STATE_READING && sm->buffer_pos > 0) {
        sm->current_state = STATE_PROCESSING;
        handle_processing(sm);
    }
    
    if (sm->current_state != STATE_ERROR) {
        sm->current_state = STATE_COMPLETE;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    char input_line[512];
    
    printf("Enter text to count words (max 511 characters): ");
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_line[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(&sm, input_line);
    
    if (sm.current_state == STATE_COMPLETE) {
        printf("Total words counted: %u\n", sm.word_count);
    } else if (sm.current_state == STATE_ERROR) {
        printf("Processing failed due to errors\n");
        return 1;
    }
    
    return 0;
}