//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10

typedef enum {
    STATE_IDLE,
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

State handle_idle_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_writing_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);
State handle_done_state(StateMachine *sm, Event event);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_length = 0;
    sm->data_processed = 0;
}

State handle_idle_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing...\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Resetting from idle state.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for idle state.\n");
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (sm->buffer_length > 0 && sm->buffer_length < MAX_INPUT_LENGTH) {
                printf("Data read successfully: %s\n", sm->buffer);
                return STATE_PROCESSING;
            } else {
                printf("Invalid data length.\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("Resetting from reading state.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for reading state.\n");
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->buffer_length > 0) {
                sm->data_processed = 1;
                printf("Data processed successfully.\n");
                return STATE_WRITING;
            } else {
                printf("No data to process.\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("Resetting from processing state.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for processing state.\n");
            return STATE_ERROR;
    }
}

State handle_writing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (sm->data_processed) {
                printf("Data written successfully.\n");
                return STATE_DONE;
            } else {
                printf("No processed data to write.\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("Resetting from writing state.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for writing state.\n");
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for error state.\n");
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for done state.\n");
            return STATE_ERROR;
    }
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, event);
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

int read_input_data(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    printf("Enter data to process (max %d characters): ", MAX_INPUT_LENGTH - 1);
    
    if (fgets(sm->buffer,