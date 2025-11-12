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

void init_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
}

int handle_idle(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return 0;
    
    if (input_len > 0) {
        if (sm->buffer_len + input_len < MAX_BUFFER) {
            strncpy(sm->buffer, input, MAX_BUFFER - 1);
            sm->buffer_len = input_len;
            sm->buffer[sm->buffer_len] = '\0';
            TRANSITION(sm, STATE_READING);
            return 1;
        }
    }
    return 0;
}

int handle_reading(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len > 0) {
        for (size_t i = 0; i < sm->buffer_len; i++) {
            if (sm->buffer[i] < 32 || sm->buffer[i] > 126) {
                TRANSITION(sm, STATE_ERROR);
                return 0;
            }
        }
        TRANSITION(sm, STATE_PROCESSING);
        return 1;
    }
    TRANSITION(sm, STATE_ERROR);
    return 0;
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len > 0) {
        for (size_t i = 0; i < sm->buffer_len; i++) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
        TRANSITION(sm, STATE_WRITING);
        return 1;
    }
    TRANSITION(sm, STATE_ERROR);
    return 0;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len > 0) {
        if (fwrite(sm->buffer, sizeof(char), sm->buffer_len, stdout) == sm->buffer_len) {
            printf("\n");
            fflush(stdout);
            TRANSITION(sm, STATE_IDLE);
            sm->buffer[0] = '\0';
            sm->buffer_len = 0;
            return 1;
        }
    }
    TRANSITION(sm, STATE_ERROR);
    return 0;
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    fprintf(stderr, "Error: Invalid state transition\n");
    TRANSITION(sm, STATE_IDLE);
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    return 0;
}

int process_state(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    switch (sm->state) {
        case STATE_IDLE:
            return 1;
        case STATE_READING:
            return handle_reading(sm);
        case STATE_PROCESSING:
            return handle_processing(sm);
        case STATE_WRITING:
            return handle_writing(sm);
        case STATE_ERROR:
            return handle_error(sm);
        default:
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 1];
    
    init_machine(&sm);
    
    printf("State Machine Demo - Enter text (empty to exit):\n");
    
    while (1) {
        if (sm.state == STATE_IDLE) {
            printf("> ");
            fflush(stdout);
            
            if (fgets(input, sizeof(input), stdin) == NULL) break;
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }
            
            if (len == 0) break;
            
            if (!handle_idle(&sm, input)) {
                TRANSITION(&sm, STATE_ERROR);
            }
        }
        
        if (!process_state(&sm)) {
            TRANSITION(&sm, STATE_ERROR);
            process_state(&sm);
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}