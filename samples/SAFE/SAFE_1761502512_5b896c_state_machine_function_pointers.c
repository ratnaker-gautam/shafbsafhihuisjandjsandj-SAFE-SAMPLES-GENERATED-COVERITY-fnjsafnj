//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    int data;
    int counter;
} StateMachine;

typedef void (*StateHandler)(StateMachine*);

void handle_idle(StateMachine* sm) {
    if (sm->current_state != STATE_IDLE) return;
    
    printf("IDLE: Enter command (start/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) {
        sm->current_state = STATE_READY;
        sm->data = 0;
        sm->counter = 0;
    } else if (strcmp(input, "quit") == 0) {
        sm->current_state = STATE_DONE;
    } else {
        printf("Invalid command\n");
    }
}

void handle_ready(StateMachine* sm) {
    if (sm->current_state != STATE_READY) return;
    
    printf("READY: Enter number (1-100) or 'back': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "back") == 0) {
        sm->current_state = STATE_IDLE;
    } else {
        char* endptr;
        long num = strtol(input, &endptr, 10);
        if (endptr != input && *endptr == '\0' && num >= 1 && num <= 100) {
            sm->data = (int)num;
            sm->current_state = STATE_PROCESSING;
        } else {
            printf("Invalid number\n");
        }
    }
}

void handle_processing(StateMachine* sm) {
    if (sm->current_state != STATE_PROCESSING) return;
    
    if (sm->data <= 0) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    printf("PROCESSING: Data=%d, Counter=%d\n", sm->data, sm->counter);
    
    if (sm->counter >= sm->data) {
        sm->current_state = STATE_READY;
        return;
    }
    
    printf("Continue? (y/n): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0) {
        if (sm->counter < 1000) {
            sm->counter++;
        } else {
            printf("Counter limit reached\n");
            sm->current_state = STATE_READY;
        }
    } else if (strcmp(input, "n") == 0 || strcmp(input, "no") == 0) {
        sm->current_state = STATE_READY;
    } else {
        printf("Invalid response\n");
    }
}

void handle_done(StateMachine* sm) {
    if (sm->current_state != STATE_DONE) return;
    printf("DONE: Exiting state machine\n");
}

void handle_error(StateMachine* sm) {
    if (sm->current_state != STATE_ERROR) return;
    printf("ERROR: Invalid state transition\n");
    sm->current_state = STATE_IDLE;
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_done,
        handle_error
    };
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (sm.current_state != STATE_DONE) {
        if (sm.current_state >= STATE_IDLE && sm.current_state <= STATE_ERROR) {
            handlers[sm.current_state](&sm);
        } else {
            sm.current_state = STATE_ERROR;
        }
    }
    
    return 0;
}