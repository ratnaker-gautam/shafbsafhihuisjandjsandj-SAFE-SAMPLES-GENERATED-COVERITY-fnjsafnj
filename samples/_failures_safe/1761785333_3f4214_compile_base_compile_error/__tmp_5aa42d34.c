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
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
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
    
    if (strcmp(input, "quit") == 0) {
        return TRANSITION_ERROR;
    }
    
    if (len >= MAX_BUFFER) {
        printf("Input too long, moving to ERROR\n");
        return change_state(sm, STATE_ERROR);
    }
    
    if (len + 1 > MAX_BUFFER) {
        return TRANSITION_ERROR;
    }
    
    memcpy(sm->buffer, input, len + 1);
    sm->buffer_len = len;
    
    return change_state(sm, STATE_READING);
}

static int handle_reading(StateMachine *sm) {
    if (sm->buffer_len >= MAX_BUFFER) {
        return TRANSITION_ERROR;
    }
    printf("Read %zu characters: '%.*s'\n", sm->buffer_len, (int)sm->buffer_len, sm->buffer);
    return change_state(sm, STATE_PROCESSING);
}

static int handle_processing(StateMachine *sm) {
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    printf("Processed to uppercase: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    return change_state(sm, STATE_WRITING);
}

static int handle_writing(StateMachine *sm) {
    printf("Writing completed: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    return change_state(sm, STATE_IDLE);
}

static int handle_error(StateMachine *sm) {
    printf("Error state: Resetting machine\n");
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    return change_state(sm, STATE_IDLE);
}

static const char *state_name(int state) {
    static const char *names[] = {"IDLE", "READING", "PROCESSING", "WRITING", "ERROR"};
    if (!CHECK_BOUNDS(state, 5)) return "UNKNOWN";
    return names[state];
}

int main(void) {
    StateMachine sm = {STATE_IDLE, {0}, 0};
    int running = 1;
    
    printf("State Machine Demo - Type 'quit' to exit\n");
    
    while (running) {
        printf("\nCurrent state: %s\n", state_name(sm.current_state));
        
        int result = TRANSITION_OK;
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
            case STATE_ERROR:
                result = handle_error(&sm);