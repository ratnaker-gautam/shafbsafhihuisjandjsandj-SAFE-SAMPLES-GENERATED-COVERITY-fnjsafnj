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
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "quit") == 0) return EVENT_ERROR;
    
    return EVENT_INVALID;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for START state\n");
            return STATE_START;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < MAX_STATES) {
                sm->data_count++;
                printf("Data received, count: %d\n", sm->data_count);
            }
            return STATE_READING;
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("Processing %d data items\n", sm->data_count);
                return STATE_PROCESSING;
            }
            printf("No data to process\n");
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for READING state\n");
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete\n");
            sm->data_count = 0;
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Restarting state machine\n");
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for FINISHED state\n");
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
            printf("System in error state, need restart\n");
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
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_FINISHED: printf("FINISHED"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        Event event = get_next_event();