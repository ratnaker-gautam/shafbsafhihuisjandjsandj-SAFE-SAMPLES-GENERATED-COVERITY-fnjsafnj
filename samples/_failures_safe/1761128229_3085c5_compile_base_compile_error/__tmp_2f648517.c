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
    size_t buffer_length;
    int data_processed;
} StateMachine;

State handle_start_state(Event event, StateMachine* sm);
State handle_reading_state(Event event, StateMachine* sm);
State handle_processing_state(Event event, StateMachine* sm);
State handle_writing_state(Event event, StateMachine* sm);
State handle_error_state(Event event, StateMachine* sm);
State handle_done_state(Event event, StateMachine* sm);

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_length = 0;
    sm->data_processed = 0;
}

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length >= MAX_INPUT_LENGTH) return 0;
    for (size_t i = 0; i < length; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

int read_input_data(StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Enter data to process (max %d chars): ", MAX_INPUT_LENGTH - 1);
    
    char input[MAX_INPUT_LENGTH];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        return 0;
    }
    
    if (len >= sizeof(sm->buffer)) {
        return 0;
    }
    
    memcpy(sm->buffer, input, len);
    sm->buffer_length = len;
    sm->buffer[len] = '\0';
    return 1;
}

int process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_length == 0) return 0;
    
    for (size_t i = 0; i < sm->buffer_length; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    sm->data_processed = 1;
    return 1;
}

int write_output_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_length == 0) return 0;
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

State handle_start_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (read_input_data(sm)) {
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (process_data(sm)) {
                return STATE_WRITING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_writing_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (write_output_data(sm)) {
                return STATE_DONE;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_done_state(Event event, StateMachine