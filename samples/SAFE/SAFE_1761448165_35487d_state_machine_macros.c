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

#define TRANSITION_OK 0
#define TRANSITION_ERROR -1

#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} StateMachine;

static int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static int transition_state(StateMachine* sm, int new_state) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (!CHECK_BOUNDS(new_state, 5)) return TRANSITION_ERROR;
    
    int valid_transition = 0;
    switch (sm->current_state) {
        case STATE_IDLE:
            valid_transition = (new_state == STATE_READING);
            break;
        case STATE_READING:
            valid_transition = (new_state == STATE_PROCESSING || new_state == STATE_ERROR);
            break;
        case STATE_PROCESSING:
            valid_transition = (new_state == STATE_WRITING || new_state == STATE_ERROR);
            break;
        case STATE_WRITING:
            valid_transition = (new_state == STATE_IDLE || new_state == STATE_ERROR);
            break;
        case STATE_ERROR:
            valid_transition = (new_state == STATE_IDLE);
            break;
        default:
            valid_transition = 0;
    }
    
    if (valid_transition) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    return TRANSITION_ERROR;
}

static int process_buffer(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (sm->current_state != STATE_PROCESSING) return TRANSITION_ERROR;
    
    if (sm->buffer_len == 0) {
        return transition_state(sm, STATE_ERROR);
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return transition_state(sm, STATE_WRITING);
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    memset(sm.buffer, 0, sizeof(sm.buffer));
    
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Demo - Enter text (empty line to exit):\n");
    
    while (1) {
        if (sm.current_state == STATE_IDLE) {
            printf("> ");
            fflush(stdout);
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }
            
            if (len == 0) {
                break;
            }
            
            if (!validate_input(input, len)) {
                transition_state(&sm, STATE_ERROR);
                continue;
            }
            
            if (len >= MAX_BUFFER) {
                transition_state(&sm, STATE_ERROR);
                continue;
            }
            
            memcpy(sm.buffer, input, len);
            sm.buffer[len] = '\0';
            sm.buffer_len = len;
            
            if (transition_state(&sm, STATE_READING) == TRANSITION_OK) {
                printf("State: READING\n");
            }
        }
        
        if (sm.current_state == STATE_READING) {
            if (transition_state(&sm, STATE_PROCESSING) == TRANSITION_OK) {
                printf("State: PROCESSING\n");
            }
        }
        
        if (sm.current_state == STATE_PROCESSING) {
            if (process_buffer(&sm) == TRANSITION_OK) {
                printf("State: WRITING\n");
            }
        }
        
        if (sm.current_state == STATE_WRITING) {
            printf("Result: %s\n", sm.buffer);
            if (transition_state(&sm, STATE_IDLE) == TRANSITION_OK) {
                printf("State: IDLE\n");
            }
        }
        
        if (sm.current_state == STATE_ERROR) {
            printf("Error: Invalid input or state transition\n");
            if (transition_state(&sm, STATE_IDLE) == TRANSITION_OK) {
                printf("State: IDLE\n");
            }
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}