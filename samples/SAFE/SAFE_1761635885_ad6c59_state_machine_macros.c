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
#define MAX_BUFFER_SIZE 50

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return -1;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return -1;
    
    switch (sm->state) {
        case STATE_IDLE:
            if (input_len > 0) {
                TRANSITION(sm, STATE_READING);
                sm->buffer_len = 0;
            }
            break;
            
        case STATE_READING:
            if (input_len == 0) {
                TRANSITION(sm, STATE_IDLE);
            } else if (sm->buffer_len + input_len < MAX_BUFFER_SIZE) {
                memcpy(sm->buffer + sm->buffer_len, input, input_len);
                sm->buffer_len += input_len;
                TRANSITION(sm, STATE_PROCESSING);
            } else {
                sm->error_code = 1;
                TRANSITION(sm, STATE_ERROR);
            }
            break;
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            TRANSITION(sm, STATE_WRITING);
            break;
            
        case STATE_WRITING:
            if (sm->buffer_len > 0) {
                printf("Processed: %.*s\n", (int)sm->buffer_len, sm->buffer);
                sm->buffer_len = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            TRANSITION(sm, STATE_IDLE);
            break;
            
        case STATE_ERROR:
            printf("Error %d occurred\n", sm->error_code);
            state_machine_init(sm);
            break;
            
        default:
            sm->error_code = 2;
            TRANSITION(sm, STATE_ERROR);
            break;
    }
    
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo - Enter text (empty line to exit):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0 && sm.state == STATE_IDLE) {
            break;
        }
        
        if (state_machine_handle_input(&sm, input) != 0) {
            fprintf(stderr, "Input handling failed\n");
            break;
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}