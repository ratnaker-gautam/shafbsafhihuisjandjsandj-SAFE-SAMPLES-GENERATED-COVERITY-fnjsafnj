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

static int validate_transition(int from_state, int to_state) {
    const int valid_transitions[5][5] = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 0, 0, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0}
    };
    
    if (!CHECK_BOUNDS(from_state, 5) || !CHECK_BOUNDS(to_state, 5)) {
        return TRANSITION_ERROR;
    }
    
    return valid_transitions[from_state][to_state] ? TRANSITION_OK : TRANSITION_ERROR;
}

static int change_state(StateMachine *sm, int new_state) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    if (validate_transition(sm->current_state, new_state) == TRANSITION_OK) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    
    return TRANSITION_ERROR;
}

static int handle_idle(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    char input[MAX_INPUT_LEN + 1];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return TRANSITION_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input, staying in IDLE\n");
        return TRANSITION_OK;
    }
    
    if (len > MAX_BUFFER - 1) {
        printf("Input too long, moving to ERROR\n");
        return change_state(sm, STATE_ERROR);
    }
    
    if (len > 0) {
        memcpy(sm->buffer, input, len);
        sm->buffer[len] = '\0';
        sm->buffer_len = len;
    }
    
    return change_state(sm, STATE_READING);
}

static int handle_reading(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("Read %zu characters: '%s'\n", sm->buffer_len, sm->buffer);
    return change_state(sm, STATE_PROCESSING);
}

static int handle_processing(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: '%s'\n", sm->buffer);
    return change_state(sm, STATE_WRITING);
}

static int handle_writing(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("Final output: %s\n", sm->buffer);
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    return change_state(sm, STATE_IDLE);
}

static int handle_error(StateMachine *sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    printf("Error state encountered. Resetting...\n");
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    return change_state(sm, STATE_IDLE);
}

static const char *state_name(int state) {
    static const char *names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (!CHECK_BOUNDS(state, 5)) return "UNKNOWN";
    return names[state];
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer[0] = '\0';
    sm.buffer_len = 0;
    
    int iterations = 0;
    const int max_iterations = 10;
    
    while (iterations < max_iterations) {
        printf("\nCurrent state: %s\n", state_name(sm.current_state));
        
        int result = TRANSITION_ERROR;
        
        switch (sm.current_state) {
            case STATE_IDLE:
                result = handle_idle(&sm);
                break;
            case STATE_READING:
                result = handle_reading(&sm);
                break;
            case STATE_PROCESSING:
                result = handle_processing(&sm);
                break;
            case STATE_WRITING:
                result = handle_writing(&sm);
                break;
            case