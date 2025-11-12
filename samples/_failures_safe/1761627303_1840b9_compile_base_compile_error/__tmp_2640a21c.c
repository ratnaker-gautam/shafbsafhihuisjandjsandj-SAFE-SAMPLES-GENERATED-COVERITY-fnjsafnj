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

int handle_idle(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    printf("System ready. Enter 'start' to begin: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return TRANSITION_ERROR;
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "start") == 0) {
        return state_transition(sm, STATE_READING);
    }
    return TRANSITION_OK;
}

int handle_reading(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    printf("Enter data (max %d chars): ", MAX_INPUT_LEN);
    char input[MAX_INPUT_LEN + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) return TRANSITION_ERROR;
    
    size_t len = strcspn(input, "\n");
    if (len >= sizeof(input) - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return state_transition(sm, STATE_ERROR);
    }
    
    if (len > MAX_BUFFER - 1) {
        return state_transition(sm, STATE_ERROR);
    }
    
    if (len > 0) {
        memcpy(sm->buffer, input, len);
        sm->buffer[len] = '\0';
        sm->buffer_len = len;
        return state_transition(sm, STATE_PROCESSING);
    }
    return state_transition(sm, STATE_ERROR);
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL || sm->buffer_len >= MAX_BUFFER) return TRANSITION_ERROR;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return state_transition(sm, STATE_WRITING);
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL || sm->buffer_len >= MAX_BUFFER) return TRANSITION_ERROR;
    
    printf("Processed data: %s\n", sm->buffer);
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    
    return state_transition(sm, STATE_IDLE);
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    printf("Error occurred. Enter 'reset' to continue: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return TRANSITION_ERROR;
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "reset") == 0) {
        sm->buffer[0] = '\0';
        sm->buffer_len = 0;
        return state_transition(sm, STATE_IDLE);
    }
    return TRANSITION_OK;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer[0] = '\0';
    sm.buffer_len = 0;
    
    int (*handlers[])(StateMachine*) = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    while (1) {
        if (!CHECK_BOUNDS(sm.current_state, 5)) {
            sm.current_state = STATE_ERROR;
        }
        
        int result = handlers[sm.current_state](&sm);
        if (result == TRANSITION_ERROR) {
            sm.current_state = STATE_ERROR;
        }
        
        if (sm.current_state == STATE_IDLE) {