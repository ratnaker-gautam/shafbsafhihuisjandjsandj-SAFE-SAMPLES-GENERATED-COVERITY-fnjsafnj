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
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
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

void handle_ready(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
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
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_SUCCESS:
            sm->current_state = STATE_COMPLETE;
            break;
        case EVENT_FAILURE:
            sm->error_code = 1;
            sm->current_state = STATE_ERROR;
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

void handle_complete(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
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
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) return;
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) return;
    
    handlers[sm->current_state](sm, event);
}

const char* state_to_string(State state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    if (state < 0 || state >= STATE_COUNT) return "UNKNOWN";
    return names[state];
}

const char* event_to_string(Event event) {
    static const char* names[] = {
        "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
    };
    
    if (event < 0 || event >= EVENT_COUNT) return "UNKNOWN";
    return names[event];
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    char input[32];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data, process, success, failure, reset, quit\n");
    
    while (1) {
        printf("\nCurrent state: %s, Data: %d, Error: %d\n", 
               state_to_string(sm.current_state), sm.data_value, sm.error_code);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) break;
        
        Event event;
        if (strcmp(input, "start") == 0) event = EVENT_START;
        else if (strcmp(input, "data") == 0) event = EVENT_DATA;
        else if (strcmp(input, "process") == 0) event = EVENT_PROCESS;
        else if (strcmp(input, "success") == 0) event = EVENT_SUCCESS;
        else if (strcmp(input, "failure") == 0) event = EVENT_FAILURE