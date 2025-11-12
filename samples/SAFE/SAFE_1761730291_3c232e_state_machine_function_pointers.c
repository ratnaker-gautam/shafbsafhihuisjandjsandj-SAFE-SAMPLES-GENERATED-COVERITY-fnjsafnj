//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
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

State handle_idle(StateMachine* sm, Event evt) {
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
    switch (evt) {
        case EVENT_DATA:
            sm->data_value = 42;
            return STATE_PROCESSING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

State handle_processing(StateMachine* sm, Event evt) {
    switch (evt) {
        case EVENT_SUCCESS:
            return STATE_DONE;
        case EVENT_FAILURE:
            sm->error_code = 1;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_done(StateMachine* sm, Event evt) {
    switch (evt) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

State handle_error(StateMachine* sm, Event evt) {
    switch (evt) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

void process_event(StateMachine* sm, Event evt) {
    if (sm == NULL || evt < 0 || evt >= EVENT_COUNT) {
        return;
    }
    StateHandler handler = state_handlers[sm->current_state];
    if (handler != NULL) {
        sm->current_state = handler(sm, evt);
    }
}

const char* state_name(State state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "DONE", "ERROR"
    };
    if (state < 0 || state >= STATE_COUNT) {
        return "UNKNOWN";
    }
    return names[state];
}

const char* event_name(Event evt) {
    static const char* names[] = {
        "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
    };
    if (evt < 0 || evt >= EVENT_COUNT) {
        return "UNKNOWN";
    }
    return names[evt];
}

int main(void) {
    StateMachine sm = {STATE_IDLE, 0, 0};
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
    
    printf("State Machine Demo\n");
    printf("Initial state: %s\n", state_name(sm.current_state));
    
    for (int i = 0; i < num_events; i++) {
        Event evt = events[i];
        printf("Processing event: %s\n", event_name(evt));
        process_event(&sm, evt);
        printf("New state: %s", state_name(sm.current_state));
        if (sm.current_state == STATE_DONE) {
            printf(" (Data: %d)", sm.data_value);
        } else if (sm.current_state == STATE_ERROR) {
            printf(" (Error: %d)", sm.error_code);
        }
        printf("\n\n");
    }
    
    return 0;
}