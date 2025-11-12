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
    char input[MAX_INPUT_LEN];
    printf("Enter command (read/quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "read") == 0) {
        TRANSITION(sm, STATE_READING);
        return 0;
    } else if (strcmp(input, "quit") == 0) {
        return 1;
    } else {
        printf("Unknown command: %s\n", input);
        return 0;
    }
}

int state_reading(StateMachine *sm) {
    char input[MAX_INPUT_LEN];
    printf("Enter data to process: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[--len] = 0;
    }
    
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    if (len + sm->buffer_len >= MAX_BUFFER) {
        printf("Buffer full\n");
        TRANSITION(sm, STATE_IDLE);
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_len, input, len);
    sm->buffer_len += len;
    sm->buffer[sm->buffer_len] = 0;
    
    TRANSITION(sm, STATE_PROCESSING);
    return 0;
}

int state_processing(StateMachine *sm) {
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_IDLE);
        return 0;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 0;
}

int state_writing(StateMachine *sm) {
    printf("Processed data: %s\n", sm->buffer);
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

int state_error(StateMachine *sm) {
    printf("Error state reached\n");
    state_machine_init(sm);
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    int result = 0;
    
    while (result == 0) {
        switch (sm.state) {
            case STATE_IDLE:
                result = state_idle(&sm);
                break;
            case STATE_READING:
                result = state_reading(&sm);
                break;
            case STATE_PROCESSING:
                result = state_processing(&sm);
                break;
            case STATE_WRITING:
                result = state_writing(&sm);
                break;
            case STATE_ERROR:
                result = state_error(&sm);
                break;
            default:
                printf("Invalid state\n");
                return 1;
        }
        
        if (result < 0) {
            printf("State machine error\n");
            return 1;
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}