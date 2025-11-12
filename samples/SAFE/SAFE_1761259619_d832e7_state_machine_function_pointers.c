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

typedef struct {
    State current_state;
    int data_value;
    int error_code;
} StateMachine;

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_ready(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            printf("Transition: READY -> PROCESSING\n");
            return STATE_PROCESSING;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

State handle_processing(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> COMPLETE\n");
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            sm->error_code = 1;
            printf("Transition: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_complete(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

State handle_error(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            sm->error_code = 0;
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
}

int process_event(StateMachine* sm, Event event, StateHandler* handlers) {
    if (sm == NULL || handlers == NULL) return 0;
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) return 0;
    if (event < 0 || event >= EVENT_COUNT) return 0;
    
    State new_state = handlers[sm->current_state](sm, event);
    if (new_state < 0 || new_state >= STATE_COUNT) return 0;
    
    sm->current_state = new_state;
    return 1;
}

int main() {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event numbers (0-5) or -1 to exit:\n");
    
    int input;
    while (1) {
        printf("Current state: %d | Enter event: ", sm.current_state);
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == -1) break;
        
        if (input < 0 || input >= EVENT_COUNT) {
            printf("Invalid event number. Must be 0-5.\n");
            continue;
        }
        
        if (!process_event(&sm, (Event)input, handlers)) {
            printf("Failed to process event.\n");
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}