//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_finished_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            sm->data_count = 0;
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < 3) {
                sm->data_count++;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_FINISHED:
            return handle_finished_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

const char *state_to_string(State state) {
    switch (state) {
        case STATE_START: return "START";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_FINISHED: return "FINISHED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char *event_to_string(Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA: return "DATA";
        case EVENT_COMPLETE: return "COMPLETE";
        case EVENT_ERROR: return "ERROR";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    int choice;
    
    initialize_state_machine(&sm);
    
    printf("State Machine Simulation\n");
    printf("Current state: %s\n", state_to_string(sm.current_state));
    
    while (sm.current_state != STATE_FINISHED && sm.current_state != STATE_ERROR) {
        printf("\nAvailable events:\n");
        printf("1. START\n");
        printf("2. DATA\n");
        printf("3. COMPLETE\n");
        printf("4. ERROR\n");
        printf("5. RESET\n");
        printf("Choose event (1-5): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {