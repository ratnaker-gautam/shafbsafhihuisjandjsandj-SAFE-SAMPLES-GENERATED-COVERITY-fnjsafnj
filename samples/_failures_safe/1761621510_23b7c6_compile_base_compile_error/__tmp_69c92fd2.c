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
    EVENT_INVALID
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

Event get_next_event(void) {
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_INVALID;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "start") == 0) {
        return EVENT_START;
    } else if (strcmp(input, "data") == 0) {
        return EVENT_DATA;
    } else if (strcmp(input, "process") == 0) {
        return EVENT_PROCESS;
    } else if (strcmp(input, "complete") == 0) {
        return EVENT_COMPLETE;
    } else if (strcmp(input, "error") == 0) {
        return EVENT_ERROR;
    } else if (strcmp(input, "quit") == 0) {
        return EVENT_INVALID;
    } else {
        return EVENT_INVALID;
    }
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            sm->data_count = 0;
            return STATE_READING;
        case EVENT_ERROR:
            printf("Error occurred\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for start state\n");
            return STATE_START;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < 3) {
                sm->data_count++;
                printf("Data received (%d/3)\n", sm->data_count);
                if (sm->data_count >= 3) {
                    return STATE_PROCESSING;
                }
            }
            return STATE_READING;
        case EVENT_ERROR:
            printf("Error during data reading\n");
            return STATE_ERROR;
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        default:
            printf("Invalid event for reading state\n");
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing completed successfully\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        case EVENT_DATA:
            printf("Cannot accept new data during processing\n");
            return STATE_PROCESSING;
        default:
            printf("Invalid event for processing state\n");
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Restarting state machine\n");
            sm->data_count = 0;
            return STATE_READING;
        case EVENT_ERROR:
            printf("Error after completion\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for finished state\n");
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Recovering from error state\n");
            sm->data_count = 0;
            return STATE_READING;
        default:
            printf("System in error state - need restart\n");
            return STATE_ERROR;
    }
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Available commands: start, data, process, complete, error\n");
    printf("Type 'quit' to exit\n");
    
    while (1) {
        printf("Current state: ");
        switch (sm->current_state) {