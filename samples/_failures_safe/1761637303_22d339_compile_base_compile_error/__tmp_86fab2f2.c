//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 8

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
    char buffer[MAX_INPUT_LEN];
    size_t data_length;
    int error_code;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_writing_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);
State handle_done_state(StateMachine *sm, Event event);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->data_length = 0;
    sm->error_code = 0;
}

State transition_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_WRITING:
            return handle_writing_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        case STATE_DONE:
            return handle_done_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started. Entering reading state.\n");
            return STATE_READING;
        default:
            return STATE_START;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Reading data...\n");
            printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
            
            if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) != NULL) {
                sm->data_length = strlen(sm->buffer);
                if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
                    sm->buffer[sm->data_length - 1] = '\0';
                    sm->data_length--;
                }
                
                if (sm->data_length > 0) {
                    printf("Read %zu characters: %s\n", sm->data_length, sm->buffer);
                    return STATE_PROCESSING;
                } else {
                    printf("No data read. Staying in reading state.\n");
                    return STATE_READING;
                }
            } else {
                printf("Error reading input.\n");
                return STATE_ERROR;
            }
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            printf("Processing data...\n");
            
            if (sm->data_length > 0 && sm->data_length < MAX_INPUT_LEN) {
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed text: %s\n", sm->buffer);
                return STATE_WRITING;
            } else {
                printf("Invalid data for processing.\n");
                return STATE_ERROR;
            }
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing output...\n");
            printf("Final result: %s\n", sm->buffer);
            printf("Data length: %zu\n", sm->data_length);
            return STATE_DONE;
        default:
            return STATE_WRITING;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting state machine.\n");
            initialize_state_machine(sm);
            return STATE_START;
        default:
            printf("Error state. Use reset to continue.\n");
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {