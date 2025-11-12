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

State handle_idle(StateMachine* sm, Event evt);
State handle_ready(StateMachine* sm, Event evt);
State handle_processing(StateMachine* sm, Event evt);
State handle_complete(StateMachine* sm, Event evt);
State handle_error(StateMachine* sm, Event evt);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
};

State handle_idle(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            sm->data_value = 0;
            sm->error_code = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA:
            sm->data_value = 42;
            return STATE_READY;
        case EVENT_PROCESS:
            if (sm->data_value > 0) {
                return STATE_PROCESSING;
            }
            return STATE_READY;
        case EVENT_RESET:
            sm->data_value = 0;
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

State handle_processing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_SUCCESS:
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            sm->error_code = 1;
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_complete(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            sm->data_value = 0;
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

State handle_error(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            sm->data_value = 0;
            sm->error_code = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

void state_machine_process(StateMachine* sm, Event evt) {
    if (sm == NULL || evt < 0 || evt >= EVENT_COUNT) return;
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) return;
    
    StateHandler handler = state_handlers[sm->current_state];
    if (handler == NULL) return;
    
    State new_state = handler(sm, evt);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        printf("State transition: %s -> %s (Event: %s)\n",
               state_names[sm->current_state],
               state_names[new_state],
               event_names[evt]);
        sm->current_state = new_state;
    }
}

void print_state_info(const StateMachine* sm) {
    if (sm == NULL) return;
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Data value: %d\n", sm->data_value);
    printf("Error code: %d\n", sm->error_code);
    printf("---\n");
}

int main() {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.error_code = 0;
    
    printf("State Machine Demo\n");
    printf("==================\n\n");
    
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_START);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_DATA);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_PROCESS);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_SUCCESS);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_RESET);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_START);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_PROCESS);
    print_state_info(&sm);
    
    state_machine_process(&sm, EVENT_FAILURE);
    print_state_info(&sm);