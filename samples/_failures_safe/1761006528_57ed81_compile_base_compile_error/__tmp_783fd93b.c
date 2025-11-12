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
    EVENT_EXIT,
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

const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READY",
    "PROCESSING",
    "COMPLETE",
    "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START",
    "DATA",
    "PROCESS",
    "SUCCESS",
    "FAILURE",
    "RESET",
    "EXIT"
};

void handle_idle(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            printf("Transition: IDLE -> READY\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event %s for state IDLE\n", event_names[event]);
            break;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            sm->data_value = 42;
            sm->current_state = STATE_READY;
            printf("Data loaded: %d\n", sm->data_value);
            break;
        case EVENT_PROCESS:
            if(sm->data_value != 0) {
                sm->current_state = STATE_PROCESSING;
                printf("Transition: READY -> PROCESSING\n");
            } else {
                printf("No data to process\n");
            }
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event %s for state READY\n", event_names[event]);
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            sm->current_state = STATE_COMPLETE;
            printf("Transition: PROCESSING -> COMPLETE\n");
            break;
        case EVENT_FAILURE:
            sm->current_state = STATE_ERROR;
            sm->error_code = 1;
            printf("Transition: PROCESSING -> ERROR\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event %s for state PROCESSING\n", event_names[event]);
            break;
    }
}

void handle_complete(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event %s for state COMPLETE\n", event_names[event]);
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event %s for state ERROR\n", event_names[event]);
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if(sm == NULL || event < 0 || event >= EVENT_COUNT) {
        printf("Invalid state machine or event\n");
        return;
    }
    
    if(sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        printf("Invalid current state\n");
        return;
    }
    
    printf("Processing event %s in state %s\n", 
           event_names[event], state_names[sm->current_state]);
    
    handlers[sm->current_state](sm, event);
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    char input[32];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET, 6: EX