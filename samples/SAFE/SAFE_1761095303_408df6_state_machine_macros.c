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

int state_idle(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    char input[MAX_INPUT_LEN];
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (input_len == 0) {
        printf("Empty input, staying in idle\n");
        return 0;
    }
    
    if (sm->buffer_len + input_len >= MAX_BUFFER) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_len, input, input_len);
    sm->buffer_len += input_len;
    sm->buffer[sm->buffer_len] = '\0';
    TRANSITION(sm, STATE_READING);
    return 1;
}

int state_reading(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    printf("Read %zu characters into buffer\n", sm->buffer_len);
    TRANSITION(sm, STATE_PROCESSING);
    return 1;
}

int state_processing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed buffer to uppercase\n");
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int state_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    printf("Output: %s\n", sm->buffer);
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

int state_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    printf("Error state reached. Resetting...\n");
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    int running = 1;
    while (running) {
        switch (sm.state) {
            case STATE_IDLE:
                if (!state_idle(&sm)) running = 0;
                break;
            case STATE_READING:
                if (!state_reading(&sm)) running = 0;
                break;
            case STATE_PROCESSING:
                if (!state_processing(&sm)) running = 0;
                break;
            case STATE_WRITING:
                if (!state_writing(&sm)) running = 0;
                break;
            case STATE_ERROR:
                if (!state_error(&sm)) running = 0;
                break;
            default:
                running = 0;
                break;
        }
    }
    
    return 0;
}