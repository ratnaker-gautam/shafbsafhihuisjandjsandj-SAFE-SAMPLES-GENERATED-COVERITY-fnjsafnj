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
    NUM_STATES
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    StateHandler handlers[NUM_STATES];
    int data_value;
    int error_code;
};

void idle_handler(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void ready_handler(void) {
    printf("State: READY - Ready to receive data\n");
}

void processing_handler(void) {
    printf("State: PROCESSING - Processing data\n");
}

void complete_handler(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void error_handler(void) {
    printf("State: ERROR - Operation failed\n");
}

void init_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
    
    sm->handlers[STATE_IDLE] = idle_handler;
    sm->handlers[STATE_READY] = ready_handler;
    sm->handlers[STATE_PROCESSING] = processing_handler;
    sm->handlers[STATE_COMPLETE] = complete_handler;
    sm->handlers[STATE_ERROR] = error_handler;
}

int handle_event(struct StateMachine *sm, Event event) {
    if (sm == NULL) return 0;
    if (event < 0 || event >= NUM_EVENTS) return 0;
    
    State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                new_state = STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                new_state = STATE_PROCESSING;
                sm->data_value = 42;
            } else if (event == EVENT_FAILURE) {
                new_state = STATE_ERROR;
                sm->error_code = 1;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                new_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                new_state = STATE_ERROR;
                sm->error_code = 2;
            }
            break;
            
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_value = 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->error_code = 0;
                sm->data_value = 0;
            }
            break;
            
        default:
            return 0;
    }
    
    if (new_state != sm->current_state) {
        sm->current_state = new_state;
        return 1;
    }
    
    return 0;
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
    
    printf("State Machine Demo\n");
    printf("Current state: ");
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
        
        if (handle_event(&sm, event)) {
            printf("State transition successful.\n");
        } else {
            printf("No state transition for this event.\n");
        }
        
        printf("Current state: ");
        sm.handlers[sm.current_state]();
        
        if (sm.current_state == STATE_COMPLETE) {
            printf("Processed data value: %d\n", sm.data_value);
        } else if (sm.current_state == STATE_ERROR) {
            printf("Error code: %d\n", sm.error_code);
        }
    }
    
    printf("Exiting state machine demo.\n");
    return 0;
}