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
#define MAX_BUFFER 50

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
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_idle(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'S') {
        TRANSITION(sm, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'E') {
        TRANSITION(sm, STATE_PROCESSING);
        return 1;
    }
    if (sm->buffer_len < MAX_BUFFER - 1) {
        sm->buffer[sm->buffer_len++] = input;
        return 1;
    }
    TRANSITION(sm, STATE_ERROR);
    return 0;
}

int handle_processing(StateMachine *sm) {
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
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_len > 0) {
        if (fwrite(sm->buffer, sizeof(char), sm->buffer_len, stdout) != sm->buffer_len) {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
        printf("\n");
    }
    TRANSITION(sm, STATE_IDLE);
    init_machine(sm);
    return 1;
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("ERROR\n");
    TRANSITION(sm, STATE_IDLE);
    init_machine(sm);
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    
    switch (sm->state) {
        case STATE_IDLE:
            return handle_idle(sm, input);
        case STATE_READING:
            return handle_reading(sm, input);
        case STATE_PROCESSING:
            return 0;
        case STATE_WRITING:
            return 0;
        case STATE_ERROR:
            return 0;
        default:
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    init_machine(&sm);
    
    printf("Enter commands (S to start, E to end, Ctrl+D to exit):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
            handle_error(&sm);
            continue;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (!process_input(&sm, input[i])) {
                break;
            }
        }
        
        if (sm.state == STATE_PROCESSING) {
            if (!handle_processing(&sm)) {
                handle_error(&sm);
            }
        }
        
        if (sm.state == STATE_WRITING) {
            if (!handle_writing(&sm)) {
                handle_error(&sm);
            }
        }
        
        if (sm.state == STATE_ERROR) {
            handle_error(&sm);
        }
    }
    
    if (ferror(stdin)) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}