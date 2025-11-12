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

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->data_length = 0;
    sm->error_code = 0;
}

int read_input_data(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    printf("Enter data to process (max %d chars): ", MAX_INPUT_LENGTH - 1);
    
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return 0;
    }
    
    sm->data_length = len;
    return 1;
}

int process_data(StateMachine *sm) {
    if (sm == NULL || sm->data_length == 0) return 0;
    
    for (size_t i = 0; i < sm->data_length; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    return 1;
}

int write_output_data(StateMachine *sm) {
    if (sm == NULL || sm->data_length == 0) return 0;
    
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (read_input_data(sm)) {
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (process_data(sm)) {
                return STATE_WRITING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_writing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (write_output_data(sm)) {
                return STATE_DONE;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
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

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    Event current_event = EVENT_START;
    
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        if (sm->current_state >= 0 && sm->current_state < sizeof(state_handlers)/sizeof(state_handlers[0])) {
            State next_state