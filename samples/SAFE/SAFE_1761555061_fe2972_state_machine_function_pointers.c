//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    StateHandler handlers[STATE_COUNT];
    int data_value;
    int error_code;
};

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

void handle_idle(void) {
    printf("State: IDLE - Waiting for start event\n");
}

void handle_ready(void) {
    printf("State: READY - Ready to receive data\n");
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data\n");
}

void handle_complete(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void handle_error(void) {
    printf("State: ERROR - Operation failed\n");
}

void init_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
    
    sm->handlers[STATE_IDLE] = handle_idle;
    sm->handlers[STATE_READY] = handle_ready;
    sm->handlers[STATE_PROCESSING] = handle_processing;
    sm->handlers[STATE_COMPLETE] = handle_complete;
    sm->handlers[STATE_ERROR] = handle_error;
}

int process_event(struct StateMachine *sm, Event event) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                next_state = STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                sm->data_value = 42;
                next_state = STATE_PROCESSING;
            } else if (event == EVENT_FAILURE) {
                sm->error_code = 1;
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                next_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                sm->error_code = 2;
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        default:
            return -1;
    }
    
    if (next_state != sm->current_state) {
        sm->current_state = next_state;
        if (sm->handlers[sm->current_state] != NULL) {
            sm->handlers[sm->current_state]();
        }
        return 0;
    }
    
    return 1;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    printf("State Machine Demo Started\n");
    sm.handlers[sm.current_state]();
    
    int running = 1;
    while (running) {
        print_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice < 0 || choice > 6) {
            printf("Invalid choice. Please select 0-6.\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        Event event = (Event)choice;
        int result = process_event(&sm, event);
        
        if (result == -1) {
            printf("Error processing event\n");
        } else if (result == 1) {
            printf("Event ignored in current state\n");
        }
    }
    
    printf("State Machine Demo Ended\n");
    return 0;
}