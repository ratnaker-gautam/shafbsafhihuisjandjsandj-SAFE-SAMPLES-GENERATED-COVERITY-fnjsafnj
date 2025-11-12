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
    static const int valid_transitions[5][5] = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 0, 0, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0}
    };
    
    if (sm == NULL) return TRANSITION_ERROR;
    if (!CHECK_BOUNDS(sm->current_state, 5)) return TRANSITION_ERROR;
    if (!CHECK_BOUNDS(new_state, 5)) return TRANSITION_ERROR;
    
    if (valid_transitions[sm->current_state][new_state]) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    
    return TRANSITION_ERROR;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return TRANSITION_ERROR;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) return TRANSITION_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (state_transition(sm, STATE_READING) == TRANSITION_OK) {
                if (input_len < MAX_BUFFER) {
                    memcpy(sm->buffer, input, input_len);
                    sm->buffer_len = input_len;
                    sm->buffer[sm->buffer_len] = '\0';
                    return TRANSITION_OK;
                }
                state_transition(sm, STATE_ERROR);
            }
            break;
            
        case STATE_READING:
            if (state_transition(sm, STATE_PROCESSING) == TRANSITION_OK) {
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                return TRANSITION_OK;
            }
            break;
            
        case STATE_PROCESSING:
            if (state_transition(sm, STATE_WRITING) == TRANSITION_OK) {
                printf("Processed: %s\n", sm->buffer);
                return TRANSITION_OK;
            }
            break;
            
        case STATE_WRITING:
            if (state_transition(sm, STATE_IDLE) == TRANSITION_OK) {
                sm->buffer_len = 0;
                return TRANSITION_OK;
            }
            break;
            
        case STATE_ERROR:
            printf("Error state - resetting\n");
            sm->current_state = STATE_IDLE;
            sm->buffer_len = 0;
            return TRANSITION_OK;
    }
    
    return TRANSITION_ERROR;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    
    char input[MAX_INPUT_LEN];
    int result;
    
    printf("State Machine Demo - Enter text (or 'quit' to exit):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        result = process_input(&sm, input);
        if (result != TRANSITION_OK) {
            printf("Processing failed\n");
            state_transition(&sm, STATE_ERROR);
        }
        
        printf("Current state: %d\n", sm.current_state);
    }
    
    return 0;
}