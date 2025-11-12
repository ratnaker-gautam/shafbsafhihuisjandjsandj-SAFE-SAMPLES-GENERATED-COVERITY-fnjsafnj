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
    if (sm == NULL) return -1;
    
    printf("Enter command (read/quit): ");
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
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
    if (sm == NULL) return -1;
    
    printf("Enter text to process (max %d chars): ", MAX_BUFFER - 1);
    
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    sm->buffer_len = strcspn(sm->buffer, "\n");
    sm->buffer[sm->buffer_len] = '\0';
    
    if (!CHECK_BOUNDS(sm->buffer_len, MAX_BUFFER)) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    if (sm->buffer_len == 0) {
        printf("Empty input, returning to idle\n");
        TRANSITION(sm, STATE_IDLE);
        return 0;
    }
    
    TRANSITION(sm, STATE_PROCESSING);
    return 0;
}

int state_processing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Processing: '");
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (CHECK_BOUNDS(i, MAX_BUFFER)) {
            putchar(sm->buffer[i]);
        }
    }
    printf("'\n");
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (CHECK_BOUNDS(i, MAX_BUFFER)) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 0;
}

int state_writing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Processed result: %s\n", sm->buffer);
    printf("Processing complete\n");
    
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

int state_error(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Error state reached\n");
    state_machine_init(sm);
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    int running = 1;
    
    while (running) {
        switch (sm.state) {
            case STATE_IDLE:
                running = (state_idle(&sm) == 0);
                break;
            case STATE_READING:
                if (state_reading(&sm) != 0) running = 0;
                break;
            case STATE_PROCESSING:
                if (state_processing(&sm) != 0) running = 0;
                break;
            case STATE_WRITING:
                if (state_writing(&sm) != 0) running = 0;
                break;
            case STATE_ERROR:
                if (state_error(&sm) != 0) running = 0;
                break;
            default:
                printf("Invalid state: %d\n", sm.state);
                running = 0;
                break;
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}