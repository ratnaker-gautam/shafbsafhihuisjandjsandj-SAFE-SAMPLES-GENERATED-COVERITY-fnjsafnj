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

Event get_next_event(StateMachine *sm) {
    if (sm == NULL) return EVENT_ERROR;
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[--len] = '\0';
    
    if (!validate_input(input, len)) return EVENT_ERROR;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Already in start state\n");
            return STATE_START;
        default:
            printf("Invalid event for start state\n");
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < 3) {
                sm->data_count++;
                printf("Data received (%d/3)\n", sm->data_count);
                return STATE_READING;
            } else {
                printf("Maximum data count reached\n");
                return STATE_PROCESSING;
            }
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("Processing data\n");
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READING;
            }
        case EVENT_RESET:
            printf("Resetting from reading state\n");
            return STATE_START;
        default:
            printf("Invalid event for reading state\n");
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Resetting from processing state\n");
            return STATE_START;
        default:
            printf("Invalid event for processing state\n");
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from finished state\n");
            return STATE_START;
        default:
            printf("Invalid event for finished state\n");
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_START;
        default:
            printf("Invalid event for error state\n");
            return STATE_ERROR;
    }
}

void process_state_transition(StateMachine *sm, Event event) {
    if (sm == NULL) return;
    
    State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_START:
            new_state = handle_start_state(sm, event);
            break;
        case STATE_READING:
            new