//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef void (*StateHandler)(void*);

struct StateMachine {
    State current_state;
    int data_value;
    int process_count;
};

void handle_idle(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: IDLE - Waiting for start event\n");
    machine->data_value = 0;
    machine->process_count = 0;
}

void handle_ready(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: READY - Enter data value (1-100): ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        machine->current_state = STATE_ERROR;
        return;
    }
    
    if (input < 1 || input > 100) {
        printf("Data value out of range\n");
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->data_value = input;
    printf("Data value %d accepted\n", machine->data_value);
}

void handle_processing(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: PROCESSING - Processing data value %d\n", machine->data_value);
    
    if (machine->data_value <= 0) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    int result = machine->data_value * 2;
    if (result < machine->data_value) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->data_value = result;
    machine->process_count++;
    
    printf("Processing complete - new value: %d\n", machine->data_value);
}

void handle_done(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: DONE - Final value: %d, Process cycles: %d\n", 
           machine->data_value, machine->process_count);
}

void handle_error(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: ERROR - Resetting state machine\n");
    machine->current_state = STATE_IDLE;
    machine->data_value = 0;
    machine->process_count = 0;
}

StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

Event get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("0 - START, 1 - DATA, 2 - PROCESS, 3 - SUCCESS, 4 - FAILURE, 5 - RESET\n");
    printf("Choose event (0-5): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        return EVENT_FAILURE;
    }
    
    if (choice < 0 || choice > 5) {
        return EVENT_FAILURE;
    }
    
    return (Event)choice;
}

int transition_state(struct StateMachine *machine, Event event) {
    State current = machine->current_state;
    State next_state = current;
    
    switch (current) {
        case STATE_IDLE:
            if (event == EVENT_START) next_state = STATE_READY;
            break;
        case STATE_READY:
            if (event == EVENT_DATA) next_state = STATE_PROCESSING;
            else if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) next_state = STATE_DONE;
            else if (event == EVENT_FAILURE) next_state = STATE_ERROR;
            else if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_DONE:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    if (next_state >= STATE_IDLE && next_state <= STATE_ERROR) {
        machine->current_state = next_state;
        return 1;
    }
    
    return 0;
}

int main(void) {
    struct StateMachine machine;
    machine.current_state = STATE_IDLE;
    machine.data_value = 0;
    machine.process_count = 0;
    
    printf("State Machine Demo - Type 6 to exit\n");
    
    while (1) {
        if (machine.current_state < STATE_IDLE || machine.current_state > STATE_ERROR) {
            machine.current_state = STATE_ERROR;
        }
        
        state_handlers[machine.current_state](&machine);
        
        Event event = get_user_event();
        if (event == 6) {
            break;
        }
        
        if (!transition_state(&machine, event)) {
            printf("Invalid transition\n");
        }
        
        if (machine.current_state == STATE_PROCESSING) {
            transition_state(&machine, EVENT_SUCCESS);
        }
    }
    
    printf("Exiting state machine demo\n");
    return 0;
}