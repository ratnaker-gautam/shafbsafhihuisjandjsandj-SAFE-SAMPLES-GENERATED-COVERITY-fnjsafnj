//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, ERROR, DONE };

struct StateMachine {
    enum State current_state;
    int data;
    int error_code;
};

typedef void (*StateHandler)(struct StateMachine*);

void handle_idle(struct StateMachine* sm) {
    if (sm->current_state != IDLE) return;
    
    printf("Enter command (start/quit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = ERROR;
        sm->error_code = 1;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) {
        sm->current_state = READY;
        sm->data = 0;
    } else if (strcmp(input, "quit") == 0) {
        sm->current_state = DONE;
    } else {
        printf("Invalid command\n");
    }
}

void handle_ready(struct StateMachine* sm) {
    if (sm->current_state != READY) return;
    
    printf("Enter number to process (1-100): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = ERROR;
        sm->error_code = 2;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    char* endptr;
    long num = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        printf("Invalid number format\n");
        return;
    }
    
    if (num < 1 || num > 100) {
        printf("Number out of range\n");
        return;
    }
    
    sm->data = (int)num;
    sm->current_state = PROCESSING;
}

void handle_processing(struct StateMachine* sm) {
    if (sm->current_state != PROCESSING) return;
    
    if (sm->data < 1 || sm->data > 100) {
        sm->current_state = ERROR;
        sm->error_code = 3;
        return;
    }
    
    int result = sm->data * sm->data;
    printf("Processing: %d squared = %d\n", sm->data, result);
    
    if (result > 10000) {
        sm->current_state = ERROR;
        sm->error_code = 4;
        return;
    }
    
    sm->current_state = READY;
}

void handle_error(struct StateMachine* sm) {
    if (sm->current_state != ERROR) return;
    
    printf("Error occurred (code %d). Resetting...\n", sm->error_code);
    sm->current_state = IDLE;
    sm->data = 0;
    sm->error_code = 0;
}

void handle_done(struct StateMachine* sm) {
    if (sm->current_state != DONE) return;
    printf("Goodbye!\n");
}

int main() {
    struct StateMachine sm;
    sm.current_state = IDLE;
    sm.data = 0;
    sm.error_code = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_error,
        handle_done
    };
    
    while (sm.current_state != DONE) {
        if (sm.current_state < 0 || sm.current_state >= (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            printf("Invalid state\n");
            break;
        }
        
        handlers[sm.current_state](&sm);
    }
    
    return 0;
}