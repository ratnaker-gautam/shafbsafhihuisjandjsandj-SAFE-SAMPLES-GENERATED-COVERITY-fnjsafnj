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
            printf("Starting data processing pipeline\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Resetting state machine\n");
            initialize_state_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data to process (max %d chars): ", MAX_INPUT_LENGTH - 1);
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                sm->buffer_length = strlen(sm->buffer);
                if (sm->buffer_length > 0 && sm->buffer[sm->buffer_length - 1] == '\n') {
                    sm->buffer[sm->buffer_length - 1] = '\0';
                    sm->buffer_length--;
                }
                if (sm->buffer_length > 0) {
                    printf("Read %zu characters\n", sm->buffer_length);
                    return STATE_PROCESSING;
                }
            }
            printf("Error reading input\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Resetting from READING state\n");
            initialize_state_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->buffer_length > 0) {
                sm->data_processed = 1;
                printf("Data processed successfully\n");
                return STATE_WRITING;
            }
            printf("No data to process\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Resetting from PROCESSING state\n");
            initialize_state_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

State handle_writing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (sm->data_processed && sm->buffer_length > 0) {
                printf("Output: ");
                for (size_t i = 0; i < sm->buffer_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        putchar(sm->buffer[i] - 32);
                    } else {
                        putchar(sm->buffer[i]);
                    }
                }
                printf("\n");
                return STATE_DONE;
            }
            printf("No processed data to write\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Resetting from WRITING state\n");
            initialize_state_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from ERROR state\n");
            initialize_state_machine(sm);
            return STATE_IDLE;
        default:
            printf("System in error state - reset required\n");
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from DONE state\n");
            initialize_state_machine(sm);
            return STATE_IDLE;
        default:
            printf("Processing complete - reset to start over\n");
            return STATE