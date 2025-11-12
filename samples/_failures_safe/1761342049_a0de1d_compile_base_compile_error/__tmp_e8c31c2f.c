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
    EVENT_PROCESS,
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

State handle_start_state(Event event, StateMachine* sm);
State handle_reading_state(Event event, StateMachine* sm);
State handle_processing_state(Event event, StateMachine* sm);
State handle_finished_state(Event event, StateMachine* sm);
State handle_error_state(Event event, StateMachine* sm);

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(Event event, StateMachine* sm) {
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

State handle_processing_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_PROCESS:
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(event, sm);
        case STATE_READING:
            return handle_reading_state(event, sm);
        case STATE_PROCESSING:
            return handle_processing_state(event, sm);
        case STATE_FINISHED:
            return handle_finished_state(event, sm);
        case STATE_ERROR:
            return handle_error_state(event, sm);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(State state) {
    switch (state) {
        case STATE_START:
            printf("Current state: START\n");
            break;
        case STATE_READING:
            printf("Current state: READING\n");
            break;
        case STATE_PROCESSING:
            printf("Current state: PROCESSING\n");
            break;
        case STATE_FINISHED:
            printf("Current state: FINISHED\n");
            break;
        case STATE_ERROR:
            printf("Current state: ERROR\n");
            break;
        default:
            printf("Current state: UNKNOWN\n");
            break;
    }
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, complete, error, reset, quit\n");
    
    while (running) {
        print_state_info(sm.current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (!validate_input(input, len)) {
            printf("Invalid input\n");
            continue;
        }
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;