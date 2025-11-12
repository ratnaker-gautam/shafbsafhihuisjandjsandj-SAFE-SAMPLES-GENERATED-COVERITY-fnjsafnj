//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define MAX_BUFFER_SIZE 128

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
                if (input_len < MAX_BUFFER_SIZE) {
                    memcpy(sm->buffer, input, input_len);
                    sm->buffer_len = input_len;
                    sm->buffer[sm->buffer_len] = '\0';
                    TRANSITION(sm, STATE_READING);
                    return 0;
                } else {
                    sm->error_code = 1;
                    TRANSITION(sm, STATE_ERROR);
                    return -1;
                }
            }
            break;
            
        case STATE_READING:
            if (strcmp(input, "process") == 0) {
                TRANSITION(sm, STATE_PROCESSING);
                return 0;
            } else if (strcmp(input, "reset") == 0) {
                state_machine_init(sm);
                return 0;
            } else {
                sm->error_code = 2;
                TRANSITION(sm, STATE_ERROR);
                return -1;
            }
            break;
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            TRANSITION(sm, STATE_WRITING);
            return 0;
            
        case STATE_WRITING:
            if (strcmp(input, "print") == 0) {
                printf("Processed: %s\n", sm->buffer);
                state_machine_init(sm);
                return 0;
            } else if (strcmp(input, "reset") == 0) {
                state_machine_init(sm);
                return 0;
            } else {
                sm->error_code = 3;
                TRANSITION(sm, STATE_ERROR);
                return -1;
            }
            break;
            
        case STATE_ERROR:
            if (strcmp(input, "reset") == 0) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        default:
            sm->error_code = 4;
            return -1;
    }
    
    return 0;
}

const char* state_to_string(int state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WRITING: return "WRITING";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo\n");
    printf("Commands: <text>, process, print, reset\n");
    printf("Current state: %s\n", state_to_string(sm.state));
    
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        int result = state_machine_handle_input(&sm, input);
        if (result != 0) {
            printf("Error: %d in state %s\n", sm.error_code, state_to_string(sm.state));
        }
        
        printf("Current state: %s", state_to_string(sm.state));
        if (sm.state == STATE_WRITING) {
            printf(" (ready to print)");
        }
        printf("\n");
    }
    
    return 0;
}