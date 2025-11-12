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
#define MAX_BUFFER 50

#define TRANSITION_OK 0
#define TRANSITION_ERROR -1

#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} StateMachine;

int state_transition(StateMachine *sm, int new_state) {
    if (sm == NULL) return TRANSITION_ERROR;
    
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
            return TRANSITION_ERROR;
    }
    
    if (valid_transition) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    return TRANSITION_ERROR;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return TRANSITION_ERROR;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        return state_transition(sm, STATE_ERROR);
    }
    
    if (state_transition(sm, STATE_READING) != TRANSITION_OK) {
        return TRANSITION_ERROR;
    }
    
    if (input_len >= MAX_BUFFER) {
        return state_transition(sm, STATE_ERROR);
    }
    
    memcpy(sm->buffer, input, input_len);
    sm->buffer[input_len] = '\0';
    sm->buffer_len = input_len;
    
    if (state_transition(sm, STATE_PROCESSING) != TRANSITION_OK) {
        return TRANSITION_ERROR;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    if (state_transition(sm, STATE_WRITING) != TRANSITION_OK) {
        return TRANSITION_ERROR;
    }
    
    printf("Processed: %s\n", sm->buffer);
    
    if (state_transition(sm, STATE_IDLE) != TRANSITION_OK) {
        return TRANSITION_ERROR;
    }
    
    return TRANSITION_OK;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    sm.buffer[0] = '\0';
    
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (empty line to exit):\n");
    
    while (1) {
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
        
        if (process_input(&sm, input) != TRANSITION_OK) {
            printf("Error processing input\n");
            state_transition(&sm, STATE_ERROR);
            state_transition(&sm, STATE_IDLE);
        }
    }
    
    return 0;
}