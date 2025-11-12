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
        case EVENT_DATA_READY:
            return STATE_PROCESSING;
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
        case EVENT_PROCESS_COMPLETE:
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_WRITING;
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

State handle_done_state(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

State process_event(Event event, StateMachine* sm) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(event, sm);
        case STATE_READING:
            return handle_reading_state(event, sm);
        case STATE_PROCESSING:
            return handle_processing_state(event, sm);
        case STATE_WRITING:
            return handle_writing_state(event, sm);
        case STATE_ERROR:
            return handle_error_state(event, sm);
        case STATE_DONE:
            return handle_done_state(event, sm);
        default:
            return STATE_ERROR;
    }
}

void simulate_data_processing(StateMachine* sm) {
    if (sm == NULL || sm->buffer_length == 0) return;
    
    for (size_t i = 0; i < sm->buffer_length; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    sm->data_processed = 1;
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LENGTH];
    
    initialize_state_machine(&sm);
    
    printf("State Machine Demo - Text Processor\n");
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LENGTH - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n') {
        input[