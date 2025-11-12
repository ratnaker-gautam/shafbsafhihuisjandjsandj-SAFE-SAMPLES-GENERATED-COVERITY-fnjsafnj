//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    if (sm->buffer_len >= MAX_BUFFER - 1) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (input == 'E') {
        if (sm->buffer_len > 0) {
            TRANSITION(sm, STATE_PROCESSING);
            return 1;
        } else {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    }
    
    if (input >= '0' && input <= '9') {
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
    
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] < '0' || sm->buffer[i] > '9') {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
        int digit = sm->buffer[i] - '0';
        if (sum > INT_MAX - digit) {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
        sum += digit;
    }
    
    int written = snprintf(sm->buffer, sizeof(sm->buffer), "%d", sum);
    if (written < 0 || (size_t)written >= sizeof(sm->buffer)) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    sm->buffer_len = strlen(sm->buffer);
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Result: %s\n", sm->buffer);
    TRANSITION(sm, STATE_IDLE);
    init_machine(sm);
    return 1;
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Error: Invalid state transition\n");
    TRANSITION(sm, STATE_IDLE);
    init_machine(sm);
    return 0;
}

int process_input(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    switch(sm->state) {
        case STATE_IDLE:
            return handle_idle(sm, input);
        case STATE_READING:
            return handle_reading(sm, input);
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
    init_machine(&sm);
    
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Commands: S (start), 0-9 (digits), E (end), Q (quit)\n");
    
    while (1) {
        printf("Current state: %d > ", sm.state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
            printf("Input too long\n");
            continue;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (input[0] == 'Q' && len == 1) {
            break;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (!process_input(&sm, input[i])) {
                break;
            }
        }
    }
    
    printf("Goodbye\n");
    return 0;
}