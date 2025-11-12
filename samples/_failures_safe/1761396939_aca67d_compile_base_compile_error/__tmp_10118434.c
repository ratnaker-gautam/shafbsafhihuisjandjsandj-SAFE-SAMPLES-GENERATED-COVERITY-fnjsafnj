//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LENGTH];
    size_t data_length;
    int error_code;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_writing_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);
State handle_done_state(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine *, Event) = {
    handle_start_state,
    handle_reading_state,
    handle_processing_state,
    handle_writing_state,
    handle_error_state,
    handle_done_state
};

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine initialized. Enter data to process: ");
            sm->data_length = 0;
            sm->error_code = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (sm->data_length > 0 && sm->data_length < MAX_INPUT_LENGTH) {
                printf("Data received: %s\n", sm->buffer);
                return STATE_PROCESSING;
            } else {
                sm->error_code = 1;
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            sm->error_code = 2;
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_length > 0) {
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed data: %s\n", sm->buffer);
                return STATE_WRITING;
            } else {
                sm->error_code = 3;
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            sm->error_code = 4;
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_writing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data processing complete. Result: %s\n", sm->buffer);
            return STATE_DONE;
        case EVENT_ERROR:
            sm->error_code = 5;
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Error %d occurred. Resetting state machine.\n", sm->error_code);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting state machine.\n");
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

int read_input(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (fgets(sm->buffer, MAX_INPUT_LENGTH, stdin) == NULL) {
        return 0;
    }
    
    sm->data_length = strlen(sm->buffer);
    if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
        sm->buffer[sm->data_length - 1] = '\0';
        sm->data_length--;
    }
    
    return sm->data_length > 0;
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    State current_state = STATE_START;
    Event current_event = EVENT_START;
    
    while (1) {
        if (current_state < 0 || current_state >= (sizeof(state_handlers) / sizeof(state_handlers[0]))) {
            break;
        }
        
        State next_state = state_handlers[current_state](sm, current_event);
        
        if (next_state == STATE_ERROR) {
            current_state =