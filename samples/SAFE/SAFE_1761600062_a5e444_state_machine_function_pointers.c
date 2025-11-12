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
    
    printf("State: IDLE - Enter command (start/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "start") == 0) {
            sm->current_state = READY;
            printf("Transitioning to READY state\n");
        } else if (strcmp(input, "quit") == 0) {
            sm->current_state = DONE;
            printf("Transitioning to DONE state\n");
        } else {
            printf("Invalid command\n");
        }
    }
}

void handle_ready(struct StateMachine* sm) {
    if (sm->current_state != READY) return;
    
    printf("State: READY - Enter data value (0-100) or 'back': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "back") == 0) {
            sm->current_state = IDLE;
            printf("Transitioning to IDLE state\n");
        } else {
            char* endptr;
            long value = strtol(input, &endptr, 10);
            
            if (endptr != input && *endptr == '\0' && value >= 0 && value <= 100) {
                sm->data = (int)value;
                sm->current_state = PROCESSING;
                printf("Transitioning to PROCESSING state\n");
            } else {
                sm->current_state = ERROR;
                sm->error_code = 1;
                printf("Invalid data - Transitioning to ERROR state\n");
            }
        }
    }
}

void handle_processing(struct StateMachine* sm) {
    if (sm->current_state != PROCESSING) return;
    
    printf("State: PROCESSING - Processing data: %d\n", sm->data);
    
    if (sm->data < 0 || sm->data > 100) {
        sm->current_state = ERROR;
        sm->error_code = 2;
        printf("Data out of bounds - Transitioning to ERROR state\n");
        return;
    }
    
    int result = sm->data * 2;
    if (result > 200) {
        result = 200;
    }
    
    printf("Processing complete. Result: %d\n", result);
    
    printf("Continue? (yes/no): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "yes") == 0) {
            sm->current_state = READY;
            printf("Transitioning to READY state\n");
        } else if (strcmp(input, "no") == 0) {
            sm->current_state = DONE;
            printf("Transitioning to DONE state\n");
        } else {
            printf("Invalid input - staying in PROCESSING state\n");
        }
    }
}

void handle_error(struct StateMachine* sm) {
    if (sm->current_state != ERROR) return;
    
    printf("State: ERROR - Error code: %d\n", sm->error_code);
    printf("Options: reset/quit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "reset") == 0) {
            sm->current_state = IDLE;
            sm->error_code = 0;
            printf("Transitioning to IDLE state\n");
        } else if (strcmp(input, "quit") == 0) {
            sm->current_state = DONE;
            printf("Transitioning to DONE state\n");
        } else {
            printf("Invalid command - staying in ERROR state\n");
        }
    }
}

void handle_done(struct StateMachine* sm) {
    if (sm->current_state != DONE) return;
    printf("State: DONE - Exiting state machine\n");
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
    
    printf("State Machine Started\n");
    
    while (sm.current_state != DONE) {
        if (sm.current_state >= 0 && sm.current_state <= DONE) {
            handlers[sm.current_state](&sm);
        } else {
            printf("Invalid state detected\n");
            break;
        }
    }
    
    printf("State Machine Terminated\n");
    return 0;
}