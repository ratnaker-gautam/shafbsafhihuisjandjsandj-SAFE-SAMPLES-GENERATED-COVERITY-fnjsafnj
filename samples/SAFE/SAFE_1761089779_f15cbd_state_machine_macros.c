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
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    char input[MAX_INPUT_LEN + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    if (len == 0) {
        printf("Empty input, staying in IDLE\n");
        return 0;
    }
    
    if (len >= MAX_BUFFER) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    sm->buffer_len = len;
    memcpy(sm->buffer, input, len);
    sm->buffer[len] = '\0';
    
    TRANSITION(sm, STATE_READING);
    return 0;
}

int state_reading(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Read %zu characters: '%.*s'\n", sm->buffer_len, (int)sm->buffer_len, sm->buffer);
    TRANSITION(sm, STATE_PROCESSING);
    return 0;
}

int state_processing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    TRANSITION(sm, STATE_WRITING);
    return 0;
}

int state_writing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Writing result: ");
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (putchar(sm->buffer[i]) == EOF) {
            TRANSITION(sm, STATE_ERROR);
            return -1;
        }
    }
    printf("\n");
    
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
                if (state_idle(&sm) != 0) running = 0;
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
                printf("Invalid state\n");
                running = 0;
                break;
        }
        
        if (sm.state == STATE_IDLE && running) {
            printf("Continue? (y/n): ");
            char response[4];
            if (fgets(response, sizeof(response), stdin) != NULL) {
                if (response[0] == 'n' || response[0] == 'N') {
                    running = 0;
                }
            } else {
                running = 0;
            }
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}