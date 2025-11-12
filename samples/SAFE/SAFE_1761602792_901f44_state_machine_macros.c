//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_OUTPUT_LEN 200

#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_pos;
    size_t output_pos;
} state_machine_t;

void init_state_machine(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->input_pos = 0;
    sm->output_pos = 0;
    memset(sm->input_buffer, 0, MAX_INPUT_LEN);
    memset(sm->output_buffer, 0, MAX_OUTPUT_LEN);
}

int process_input(state_machine_t *sm, char input_char) {
    if (sm == NULL) return -1;
    
    if (!IS_VALID_CHAR(input_char)) {
        sm->state = STATE_ERROR;
        return -1;
    }

    switch (sm->state) {
        case STATE_IDLE:
            if (input_char == 'S') {
                sm->state = STATE_READING;
                sm->input_pos = 0;
                memset(sm->input_buffer, 0, MAX_INPUT_LEN);
            }
            break;
            
        case STATE_READING:
            if (input_char == 'E') {
                if (sm->input_pos > 0) {
                    sm->state = STATE_PROCESSING;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else {
                if (CHECK_BOUNDS(sm->input_pos, MAX_INPUT_LEN - 1)) {
                    sm->input_buffer[sm->input_pos++] = input_char;
                } else {
                    sm->state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_PROCESSING:
            sm->output_pos = 0;
            for (size_t i = 0; i < sm->input_pos; i++) {
                if (CHECK_BOUNDS(sm->output_pos, MAX_OUTPUT_LEN - 1)) {
                    sm->output_buffer[sm->output_pos++] = sm->input_buffer[i] + 1;
                } else {
                    sm->state = STATE_ERROR;
                    return -1;
                }
            }
            sm->state = STATE_WRITING;
            break;
            
        case STATE_WRITING:
            sm->state = STATE_IDLE;
            break;
            
        case STATE_ERROR:
            break;
            
        default:
            sm->state = STATE_ERROR;
            break;
    }
    
    return 0;
}

void print_output(const state_machine_t *sm) {
    if (sm == NULL) return;
    if (sm->state == STATE_WRITING && sm->output_pos > 0) {
        printf("Processed: %s\n", sm->output_buffer);
    } else if (sm->state == STATE_ERROR) {
        printf("Error occurred\n");
    }
}

int main(void) {
    state_machine_t sm;
    char input_char;
    int result;
    
    init_state_machine(&sm);
    
    printf("Enter characters (S to start, E to end processing):\n");
    
    while (sm.state != STATE_ERROR) {
        result = scanf("%c", &input_char);
        if (result != 1) {
            break;
        }
        
        if (input_char == '\n') {
            continue;
        }
        
        if (process_input(&sm, input_char) != 0) {
            break;
        }
        
        print_output(&sm);
        
        if (sm.state == STATE_WRITING) {
            init_state_machine(&sm);
        }
    }
    
    if (sm.state == STATE_ERROR) {
        printf("Final state: ERROR\n");
        return 1;
    }
    
    return 0;
}