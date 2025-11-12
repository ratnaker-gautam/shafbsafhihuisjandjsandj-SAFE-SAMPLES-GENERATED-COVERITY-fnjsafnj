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
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READY;
                return 0;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                sm->data_value = 42;
                sm->current_state = STATE_PROCESSING;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                sm->current_state = STATE_COMPLETE;
                return 0;
            } else if (event == EVENT_FAILURE) {
                sm->error_code = 1;
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_value = 0;
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_value = 0;
                sm->error_code = 0;
                return 0;
            }
            break;
            
        default:
            break;
    }
    
    return -1;
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
    printf("Choose an event: ");
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    int running = 1;
    int choice;
    
    printf("State Machine Demo\n");
    
    while (running) {
        sm.handlers[sm.current_state]();
        print_menu();
        
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
        if (handle_event(&sm, event) != 0) {
            printf("Invalid event for current state.\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}