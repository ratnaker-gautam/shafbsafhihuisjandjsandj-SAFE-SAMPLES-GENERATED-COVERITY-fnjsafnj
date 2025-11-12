//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
            sm->data_value = 0;
            sm->error_code = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            sm->data_value = 42;
            return STATE_PROCESSING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

State handle_processing(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            sm->error_code = 1;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_complete(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

State handle_error(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    free(sm);
}

int process_event(StateMachine* sm, Event event) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return -1;
    }

    static StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };

    State old_state = sm->current_state;
    State new_state = handlers[old_state](sm, event);
    
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return -1;
    }
    
    sm->current_state = new_state;
    return 0;
}

const char* state_to_string(State state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    if (state < 0 || state >= STATE_COUNT) {
        return "UNKNOWN";
    }
    return names[state];
}

const char* event_to_string(Event event) {
    static const char* names[] = {
        "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
    };
    if (event < 0 || event >= EVENT_COUNT) {
        return "UNKNOWN";
    }
    return names[event];
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    Event events[] = {
        EVENT_START,
        EVENT_DATA,
        EVENT_SUCCESS,
        EVENT_RESET,
        EVENT_START,
        EVENT_DATA,
        EVENT_FAILURE,
        EVENT_RESET
    };
    
    int num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    for (int i = 0; i < num_events; i++) {
        Event current_event = events[i];
        printf("Processing event: %s\n", event_to_string(current_event));
        
        if (process_event(sm, current_event) != 0) {
            printf("Error processing event\n");
            break;
        }
        
        printf("New state: %s", state_to_string(sm->current_state));
        if (sm->current_state == STATE_COMPLETE) {
            printf(" (Data: %d)", sm->data_value);
        } else if (sm->current_state == STATE_ERROR) {
            printf(" (Error: %d)", sm->error_code);
        }
        printf("\n\n");
    }
    
    destroy_state_machine(sm);
    return 0;
}