//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define INPUT_SIZE 128

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_DONE
} State;

typedef enum {
    INPUT_CHAR,
    INPUT_SPACE,
    INPUT_NEWLINE,
    INPUT_EOF
} InputType;

typedef struct {
    State current_state;
    char *buffer;
    size_t buffer_size;
    size_t buffer_pos;
    size_t word_count;
} StateMachine;

InputType get_input_type(int c) {
    if (c == EOF) return INPUT_EOF;
    if (c == '\n') return INPUT_NEWLINE;
    if (c == ' ') return INPUT_SPACE;
    if (c >= 0 && c <= 127) return INPUT_CHAR;
    return INPUT_EOF;
}

State transition_state(State current, InputType input, StateMachine *sm) {
    switch (current) {
        case STATE_START:
            if (input == INPUT_CHAR) return STATE_READING;
            return STATE_START;
        
        case STATE_READING:
            if (input == INPUT_SPACE || input == INPUT_NEWLINE) {
                sm->word_count++;
                return STATE_PROCESSING;
            }
            if (input == INPUT_EOF) {
                sm->word_count++;
                return STATE_DONE;
            }
            return STATE_READING;
        
        case STATE_PROCESSING:
            if (input == INPUT_CHAR) return STATE_READING;
            if (input == INPUT_EOF) return STATE_DONE;
            return STATE_PROCESSING;
        
        case STATE_WRITING:
            return STATE_DONE;
        
        case STATE_DONE:
            return STATE_DONE;
    }
    return STATE_START;
}

void process_state(State state, StateMachine *sm, int c) {
    switch (state) {
        case STATE_READING:
            if (sm->buffer_pos < sm->buffer_size - 1) {
                *(sm->buffer + sm->buffer_pos) = (char)c;
                sm->buffer_pos++;
                *(sm->buffer + sm->buffer_pos) = '\0';
            }
            break;
        
        case STATE_PROCESSING:
            if (sm->buffer_pos > 0) {
                printf("Word %zu: '", sm->word_count);
                char *ptr = sm->buffer;
                while (*ptr != '\0') {
                    putchar(*ptr);
                    ptr++;
                }
                printf("'\n");
                sm->buffer_pos = 0;
                *(sm->buffer) = '\0';
            }
            break;
        
        case STATE_WRITING:
        case STATE_DONE:
        case STATE_START:
            break;
    }
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.buffer_size = INPUT_SIZE;
    sm.buffer_pos = 0;
    sm.word_count = 0;
    
    sm.buffer = malloc(sm.buffer_size);
    if (sm.buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    *(sm.buffer) = '\0';
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF && sm.current_state != STATE_DONE) {
        if (c == EOF) break;
        
        InputType input_type = get_input_type(c);
        State new_state = transition_state(sm.current_state, input_type, &sm);
        
        if (new_state != sm.current_state) {
            process_state(sm.current_state, &sm, c);
            sm.current_state = new_state;
        }
        
        if (sm.current_state == STATE_READING && input_type == INPUT_CHAR) {
            process_state(sm.current_state, &sm, c);
        }
    }
    
    if (sm.current_state == STATE_READING) {
        sm.word_count++;
        process_state(STATE_PROCESSING, &sm, 0);
    }
    
    printf("Total words: %zu\n", sm.word_count);
    
    free(sm.buffer);
    return EXIT_SUCCESS;
}