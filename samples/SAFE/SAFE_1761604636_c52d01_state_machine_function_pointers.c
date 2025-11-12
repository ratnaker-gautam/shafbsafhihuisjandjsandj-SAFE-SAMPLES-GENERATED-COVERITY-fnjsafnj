//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, COMPLETE, ERROR };

struct StateMachine {
    enum State current_state;
    int data;
    int counter;
};

typedef void (*StateHandler)(struct StateMachine*);

void handle_idle(struct StateMachine* sm) {
    if (sm->current_state != IDLE) return;
    
    printf("IDLE: Enter command (start/exit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "start") == 0) {
            sm->current_state = READY;
            printf("Transitioning to READY state\n");
        } else if (strcmp(input, "exit") == 0) {
            printf("Exiting state machine\n");
            exit(0);
        } else {
            printf("Invalid command\n");
        }
    }
}

void handle_ready(struct StateMachine* sm) {
    if (sm->current_state != READY) return;
    
    printf("READY: Enter data value (0-100): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        char* endptr;
        long value = strtol(input, &endptr, 10);
        
        if (endptr != input && value >= 0 && value <= 100) {
            sm->data = (int)value;
            sm->current_state = PROCESSING;
            printf("Transitioning to PROCESSING state\n");
        } else {
            printf("Invalid data value\n");
        }
    }
}

void handle_processing(struct StateMachine* sm) {
    if (sm->current_state != PROCESSING) return;
    
    if (sm->data < 0 || sm->data > 100) {
        sm->current_state = ERROR;
        printf("Data validation failed\n");
        return;
    }
    
    printf("PROCESSING: Data %d\n", sm->data);
    
    if (sm->data > 50) {
        sm->counter = sm->data * 2;
        if (sm->counter < sm->data) {
            sm->current_state = ERROR;
            printf("Integer overflow detected\n");
            return;
        }
    } else {
        sm->counter = sm->data + 10;
    }
    
    sm->current_state = COMPLETE;
    printf("Transitioning to COMPLETE state\n");
}

void handle_complete(struct StateMachine* sm) {
    if (sm->current_state != COMPLETE) return;
    
    printf("COMPLETE: Result = %d\n", sm->counter);
    printf("Continue? (yes/no): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "yes") == 0) {
            sm->current_state = IDLE;
            sm->data = 0;
            sm->counter = 0;
            printf("Returning to IDLE state\n");
        } else if (strcmp(input, "no") == 0) {
            printf("Exiting state machine\n");
            exit(0);
        } else {
            printf("Invalid response\n");
        }
    }
}

void handle_error(struct StateMachine* sm) {
    if (sm->current_state != ERROR) return;
    
    printf("ERROR: Resetting state machine\n");
    sm->current_state = IDLE;
    sm->data = 0;
    sm->counter = 0;
    printf("Returning to IDLE state\n");
}

int main() {
    struct StateMachine sm = {IDLE, 0, 0};
    StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    printf("State Machine Started\n");
    
    while (1) {
        if (sm.current_state >= IDLE && sm.current_state <= ERROR) {
            handlers[sm.current_state](&sm);
        } else {
            printf("Invalid state detected\n");
            break;
        }
    }
    
    return 0;
}