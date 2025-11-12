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
    if (!CHECK_BOUNDS(sm->current_state, 5) || !CHECK_BOUNDS(new_state, 5)) {
        return TRANSITION_ERROR;
    }
    
    if (valid_transitions[sm->current_state][new_state]) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    
    return TRANSITION_ERROR;
}

int handle_idle(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    printf("IDLE: Ready for input\n");
    return state_transition(sm, STATE_READING);
}

int handle_reading(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    char input[MAX_INPUT_LEN];
    printf("READING: Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return state_transition(sm, STATE_ERROR);
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("READING: Empty input, returning to idle\n");
        return state_transition(sm, STATE_IDLE);
    }
    
    if (len >= MAX_BUFFER) {
        printf("READING: Input too long\n");
        return state_transition(sm, STATE_ERROR);
    }
    
    memcpy(sm->buffer, input, len + 1);
    sm->buffer_len = len;
    
    return state_transition(sm, STATE_PROCESSING);
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("PROCESSING: Processing '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return state_transition(sm, STATE_WRITING);
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("WRITING: Result: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    sm->buffer_len = 0;
    sm->buffer[0] = '\0';
    
    return state_transition(sm, STATE_IDLE);
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("ERROR: Reset required\n");
    sm->buffer_len = 0;
    sm->buffer[0] = '\0';
    
    return state_transition(sm, STATE_IDLE);
}

int process_state(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle(sm);
        case STATE_READING:
            return handle_reading(sm);
        case STATE_PROCESSING:
            return handle_processing(sm);
        case STATE_WRITING:
            return handle_writing(sm);
        case STATE_ERROR:
            return handle_error(sm);
        default:
            return TRANSITION_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    sm.buffer[0] = '\0';
    
    int cycles = 0;
    const int max_cycles = 10;
    
    while (cycles < max_cycles) {
        if (process_state(&sm) != TRANSITION_OK) {
            printf("State transition failed\n");
            break;
        }
        
        cycles++;
        
        if (sm.current_state == STATE_IDLE) {
            printf("Continue? (y/n): ");
            char response[10];
            if (fgets(response, sizeof(response), stdin) != NULL) {
                if (response[0] == 'n' || response[0] == 'N') {