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

typedef void (*StateHandler)(StateMachine*, Event);

void handle_idle(StateMachine* sm, Event event);
void handle_ready(StateMachine* sm, Event event);
void handle_processing(StateMachine* sm, Event event);
void handle_complete(StateMachine* sm, Event event);
void handle_error(StateMachine* sm, Event event);

StateHandler handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void handle_idle(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        case EVENT_RESET:
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 1;
            break;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            sm->data_value = 42;
            sm->current_state = STATE_PROCESSING;
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 2;
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_PROCESS:
            if(sm->data_value > 0) {
                sm->data_value *= 2;
                sm->current_state = STATE_COMPLETE;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 3;
            }
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 4;
            break;
    }
}

void handle_complete(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            printf("Processing complete. Result: %d\n", sm->data_value);
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 5;
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_FAILURE:
            printf("Error occurred. Code: %d\n", sm->error_code);
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if(sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return;
    }
    if(sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 6;
        return;
    }
    handlers[sm->current_state](sm, event);
}

const char* state_to_string(State state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    if(state < 0 || state >= STATE_COUNT) {
        return "UNKNOWN";
    }
    return names[state];
}

const char* event_to_string(Event event) {
    static const char* names[] = {
        "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
    };
    if(event < 0 || event >= EVENT_COUNT) {
        return "UNKNOWN";
    }
    return names[event];
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    char input[32];
    Event events[] = {
        EVENT_START,
        EVENT_DATA,
        EVENT_PROCESS,
        EVENT_SUCCESS,
        EVENT_RESET,
        EVENT_START,
        EVENT_DATA,
        EVENT_PROCESS,
        EVENT_FAILURE
    };
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE,