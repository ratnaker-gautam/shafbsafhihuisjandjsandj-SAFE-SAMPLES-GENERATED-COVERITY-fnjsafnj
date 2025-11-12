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

State handle_start_state(StateMachine* sm, Event event);
State handle_reading_state(StateMachine* sm, Event event);
State handle_processing_state(StateMachine* sm, Event event);
State handle_writing_state(StateMachine* sm, Event event);
State handle_error_state(StateMachine* sm, Event event);
State handle_done_state(StateMachine* sm, Event event);

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->data_length = 0;
    sm->error_code = 0;
}

State transition_state(StateMachine* sm, Event event) {
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

State handle_start_state(StateMachine* sm, Event event) {
    if (event == EVENT_START) {
        printf("State machine started\n");
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading_state(StateMachine* sm, Event event) {
    if (event == EVENT_DATA_READY) {
        printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
        
        if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
            sm->data_length = strlen(sm->buffer);
            if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
                sm->buffer[sm->data_length - 1] = '\0';
                sm->data_length--;
            }
            
            if (sm->data_length > 0) {
                printf("Read %zu characters\n", sm->data_length);
                return STATE_PROCESSING;
            }
        }
        sm->error_code = 1;
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing_state(StateMachine* sm, Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        if (sm->data_length > 0 && sm->data_length < sizeof(sm->buffer)) {
            for (size_t i = 0; i < sm->data_length; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            printf("Processed data: %s\n", sm->buffer);
            return STATE_WRITING;
        }
        sm->error_code = 2;
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing_state(StateMachine* sm, Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully\n");
        return STATE_DONE;
    }
    return STATE_WRITING;
}

State handle_error_state(StateMachine* sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine\n");
        initialize_state_machine(sm);
        return STATE_START;
    }
    printf("Error occurred (code: %d)\n", sm->error_code);
    return STATE_ERROR;
}

State handle_done_state(StateMachine* sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine\n");
        initialize_state_machine(sm);
        return STATE_START;
    }
    printf("Processing complete\n");
    return STATE_DONE;
}

Event get_user_event(State current_state) {
    char input[10];
    int choice = 0;
    
    switch (current_state) {
        case STATE_START:
            printf("Enter 1 to start: ");
            break;
        case STATE_READING:
            printf("Enter 1 when data is ready: ");
            break;
        case STATE_PROCESSING:
            printf("Enter 1 when processing complete: ");
            break;
        case STATE_WRITING:
            printf("Enter 1 when writing complete: ");
            break;
        case STATE_ERROR:
        case STATE_DONE:
            printf("Enter 1 to reset, 2 to exit: ");
            break;