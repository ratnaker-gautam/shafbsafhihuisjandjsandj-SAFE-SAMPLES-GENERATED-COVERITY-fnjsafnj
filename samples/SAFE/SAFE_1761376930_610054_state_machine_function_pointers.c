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

int is_valid_event(Event event) {
    return event >= EVENT_START && event < EVENT_COUNT;
}

int is_valid_state(State state) {
    return state >= STATE_IDLE && state < STATE_COUNT;
}

int handle_event(struct StateMachine *sm, Event event) {
    if (sm == NULL || !is_valid_state(sm->current_state) || !is_valid_event(event)) {
        return 0;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READY;
                sm->data_value = 0;
                sm->error_code = 0;
                return 1;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                sm->data_value = 42;
                sm->current_state = STATE_PROCESSING;
                return 1;
            } else if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                return 1;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                sm->current_state = STATE_COMPLETE;
                return 1;
            } else if (event == EVENT_FAILURE) {
                sm->error_code = 1;
                sm->current_state = STATE_ERROR;
                return 1;
            } else if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                return 1;
            }
            break;
            
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                return 1;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                return 1;
            }
            break;
            
        default:
            break;
    }
    
    return 0;
}

void run_state_machine(struct StateMachine *sm) {
    if (sm == NULL || !is_valid_state(sm->current_state)) return;
    
    if (sm->handlers[sm->current_state] != NULL) {
        sm->handlers[sm->current_state]();
    }
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=EXIT\n");
    
    int running = 1;
    while (running) {
        run_state_machine(&sm);
        
        printf("Enter event number: ");
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == 6) {
            running = 0;
            continue;
        }
        
        if (input < 0 || input >= EVENT_COUNT) {
            printf("Invalid event number. Please try again.\n");
            continue;
        }
        
        Event event = (Event)input;
        if (!handle_event(&sm, event)) {
            printf("Event %d not valid in current state.\n", input);
        }
        
        printf("\n");
    }
    
    printf("State machine terminated.\n");
    return 0;
}