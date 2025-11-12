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
    EVENT_ERROR
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

State (*state_handlers[])(StateMachine *, Event) = {
    handle_start_state,
    handle_reading_state,
    handle_processing_state,
    handle_finished_state,
    handle_error_state
};

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
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Data received: %s\n", sm->buffer);
                sm->data_count++;
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data (%d items)\n", sm->data_count);
            if (sm->data_count >= 3) {
                return STATE_FINISHED;
            } else {
                return STATE_READING;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete\n");
            return STATE_FINISHED;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    printf("Error state reached\n");
    return STATE_ERROR;
}

Event get_next_event(StateMachine *sm) {
    if (sm == NULL) return EVENT_ERROR;
    
    static int call_count = 0;
    call_count++;
    
    if (call_count == 1) return EVENT_START;
    
    if (sm->current_state == STATE_READING) {
        char input[MAX_INPUT_LEN];
        printf("Enter data (or 'quit' to finish): ");
        
        if (fgets(input, sizeof(input), stdin) != NULL) {
            size_t len = strlen(input);
            if (len > 0 && input[len-1] == '\n') {
                input[len-1] = '\0';
                len--;
            }
            
            if (validate_input(input, len)) {
                if (strcmp(input, "quit") == 0) {
                    return EVENT_PROCESS;
                } else {
                    if (len < sizeof(sm->buffer)) {
                        strncpy(sm->buffer, input, sizeof(sm->buffer)-1);
                        sm->buffer[sizeof(sm->buffer)-1] = '\0';
                        sm->buffer_len = len;
                        return EVENT_DATA;
                    }
                }
            }
        }
        return EVENT_ERROR;
    }
    
    if (sm->current_state == STATE_PROCESSING) {
        return EVENT_PROCESS;
    }
    
    if (sm->current_state == STATE_FINISHED) {
        return EVENT_COMPLETE;
    }
    
    return EVENT_ERROR;
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    int max_iterations = 20;
    int iteration = 0;
    
    while (sm->current_state != STATE_FINISHED && 
           sm->current_state != STATE_ERROR &&
           iteration < max_iterations) {
        
        Event event = get_next_event(sm);
        
        if (sm->