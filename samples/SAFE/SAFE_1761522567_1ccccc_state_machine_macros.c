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
#define MAX_BUFFER 256

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }

    switch (sm->state) {
        case STATE_IDLE:
            if (input_len > 0) {
                if (input_len < sizeof(sm->buffer)) {
                    memcpy(sm->buffer, input, input_len);
                    sm->buffer_len = input_len;
                    sm->buffer[sm->buffer_len] = '\0';
                    TRANSITION(sm, STATE_READING);
                    return 0;
                } else {
                    TRANSITION(sm, STATE_ERROR);
                    return -1;
                }
            }
            break;
            
        case STATE_READING:
            if (input_len == 0) {
                TRANSITION(sm, STATE_PROCESSING);
                return 0;
            } else {
                size_t new_len = sm->buffer_len + input_len;
                if (CHECK_BOUNDS(new_len, sizeof(sm->buffer))) {
                    memcpy(sm->buffer + sm->buffer_len, input, input_len);
                    sm->buffer_len = new_len;
                    sm->buffer[sm->buffer_len] = '\0';
                    return 0;
                } else {
                    TRANSITION(sm, STATE_ERROR);
                    return -1;
                }
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
            printf("Processed: %s\n", sm->buffer);
            TRANSITION(sm, STATE_IDLE);
            state_machine_init(sm);
            return 0;
            
        case STATE_ERROR:
            printf("Error state reached\n");
            state_machine_init(sm);
            TRANSITION(sm, STATE_IDLE);
            return -1;
            
        default:
            TRANSITION(sm, STATE_ERROR);
            return -1;
    }
    
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo - Enter text (empty line to process, 'quit' to exit):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        int result = state_machine_process_input(&sm, input);
        if (result != 0) {
            printf("Processing failed\n");
        }
        
        printf("Current state: %d\n", sm.state);
    }
    
    return 0;
}