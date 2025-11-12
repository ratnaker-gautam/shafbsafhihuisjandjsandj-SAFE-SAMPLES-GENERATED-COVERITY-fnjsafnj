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

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
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
                char input[MAX_INPUT_LEN];
                printf("Enter data (or 'quit' to finish): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len-1] == '\n') {
                        input[len-1] = '\0';
                        len--;
                    }
                    
                    if (strcmp(input, "quit") == 0) {
                        return STATE_PROCESSING;
                    }
                    
                    if (sm->buffer_len + len < MAX_INPUT_LEN - 1) {
                        strncpy(sm->buffer + sm->buffer_len, input, len);
                        sm->buffer_len += len;
                        sm->data_count++;
                        printf("Data added. Total entries: %d\n", sm->data_count);
                    } else {
                        printf("Buffer full, processing data\n");
                        return STATE_PROCESSING;
                    }
                }
            }
            return STATE_READING;
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
            printf("Processing %d data entries\n", sm->data_count);
            if (sm->data_count > 0) {
                printf("Data collected: %s\n", sm->buffer);
                printf("Total characters: %zu\n", sm->buffer_len);
            } else {
                printf("No data to process\n");
            }
            return STATE_FINISHED;
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
    
    printf("Error state encountered\n");
    return STATE_ERROR;
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

Event get_next_event(StateMachine *sm) {
    if (sm == NULL) return EVENT_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA;
        case STATE_PROCESSING:
            return EVENT_PROCESS;
        case STATE_FINISHED:
            return EVENT_COMPLETE;
        default:
            return EVENT_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    int max_iterations = 100;
    int iteration = 0;
    
    while (sm.current_state != STATE_FINISHED && 
           sm.current_state != STATE_ERROR &&
           iteration < max_iterations) {
        
        Event event = get_next_event(&sm);
        State new_state = process_event(&sm, event);
        sm.current_state = new