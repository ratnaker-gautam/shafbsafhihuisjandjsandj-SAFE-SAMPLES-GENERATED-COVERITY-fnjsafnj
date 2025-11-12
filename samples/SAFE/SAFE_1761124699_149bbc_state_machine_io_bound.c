//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
};

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->word_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_char(char c) {
    return isprint((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int process_buffer(struct StateMachine *sm) {
    if (sm->buffer_pos == 0) {
        return 0;
    }
    
    int in_word = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] == ' ' || sm->buffer[i] == '\t' || sm->buffer[i] == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            sm->word_count++;
        }
    }
    
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

int handle_input(struct StateMachine *sm, char input) {
    if (!is_valid_char(input)) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == ' ' || input == '\t' || input == '\n') {
                return 1;
            }
            sm->current_state = STATE_READING;
            break;
            
        case STATE_READING:
            if (input == '\n') {
                sm->current_state = STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            break;
            
        case STATE_DONE:
            break;
            
        case STATE_ERROR:
            return 0;
    }
    
    if (sm->current_state == STATE_READING) {
        if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
            sm->buffer[sm->buffer_pos++] = input;
        } else {
            sm->current_state = STATE_ERROR;
            return 0;
        }
    }
    
    if (sm->current_state == STATE_PROCESSING) {
        if (!process_buffer(sm)) {
            sm->current_state = STATE_ERROR;
            return 0;
        }
        sm->current_state = STATE_DONE;
    }
    
    return 1;
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    printf("Enter text (press Enter twice to finish):\n");
    
    int consecutive_newlines = 0;
    int ch;
    
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            consecutive_newlines++;
            if (consecutive_newlines >= 2) {
                break;
            }
        } else {
            consecutive_newlines = 0;
        }
        
        if (!handle_input(&sm, (char)ch)) {
            fprintf(stderr, "Error: Invalid input or buffer overflow\n");
            return 1;
        }
        
        if (sm.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (sm.current_state == STATE_ERROR) {
        fprintf(stderr, "Error: State machine entered error state\n");
        return 1;
    }
    
    if (sm.current_state == STATE_READING || sm.current_state == STATE_PROCESSING) {
        if (sm.buffer_pos > 0) {
            if (!process_buffer(&sm)) {
                fprintf(stderr, "Error: Failed to process buffer\n");
                return 1;
            }
        }
    }
    
    printf("Word count: %d\n", sm.word_count);
    return 0;
}