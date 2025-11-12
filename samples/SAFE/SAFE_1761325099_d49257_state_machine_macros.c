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
#define STATE_DONE 5

#define MAX_INPUT_LEN 100
#define MAX_BUFFER_SIZE 50

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_pos;
    int input_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->input_count = 0;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) return 0;
    
    switch (sm->state) {
        case STATE_IDLE:
            if (input_len > 0) {
                TRANSITION(sm, STATE_READING);
                sm->input_count = 0;
            }
            break;
            
        case STATE_READING:
            if (input_len > 0 && CHECK_BOUNDS(sm->buffer_pos, MAX_BUFFER_SIZE - input_len)) {
                memcpy(sm->buffer + sm->buffer_pos, input, input_len);
                sm->buffer_pos += input_len;
                sm->input_count++;
                
                if (sm->input_count >= 3 || sm->buffer_pos >= MAX_BUFFER_SIZE - 10) {
                    TRANSITION(sm, STATE_PROCESSING);
                }
            } else {
                TRANSITION(sm, STATE_ERROR);
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->buffer_pos > 0) {
                for (size_t i = 0; i < sm->buffer_pos && CHECK_BOUNDS(i, MAX_BUFFER_SIZE); i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                TRANSITION(sm, STATE_WRITING);
            } else {
                TRANSITION(sm, STATE_ERROR);
            }
            break;
            
        case STATE_WRITING:
            if (sm->buffer_pos > 0) {
                printf("Processed: %.*s\n", (int)sm->buffer_pos, sm->buffer);
                sm->buffer_pos = 0;
                sm->input_count = 0;
                TRANSITION(sm, STATE_DONE);
            } else {
                TRANSITION(sm, STATE_ERROR);
            }
            break;
            
        case STATE_ERROR:
            printf("Error occurred in state machine\n");
            initialize_machine(sm);
            break;
            
        case STATE_DONE:
            printf("Processing complete. Ready for new input.\n");
            TRANSITION(sm, STATE_IDLE);
            break;
            
        default:
            TRANSITION(sm, STATE_ERROR);
            break;
    }
    
    return 1;
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf("State Machine Demo - Enter text (empty line to quit):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            break;
        }
        
        if (!process_input(&sm, input)) {
            printf("Input processing failed\n");
        }
        
        printf("Current state: %d\n", sm.state);
    }
    
    printf("State Machine terminated\n");
    return 0;
}