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
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event, const char *data);
State handle_reading_state(StateMachine *sm, Event event, const char *data);
State handle_processing_state(StateMachine *sm, Event event, const char *data);
State handle_finished_state(StateMachine *sm, Event event, const char *data);
State handle_error_state(StateMachine *sm, Event event, const char *data);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && validate_input(data, strlen(data))) {
                size_t data_len = strlen(data);
                if (sm->buffer_len + data_len < MAX_INPUT_LEN - 1) {
                    strncpy(sm->buffer + sm->buffer_len, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    printf("Data received: %s (Total: %zu chars)\n", data, sm->buffer_len);
                    return STATE_READING;
                } else {
                    printf("Buffer full, moving to processing.\n");
                    return STATE_PROCESSING;
                }
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            if (sm->data_count > 0) {
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Processed %d data items.\n", sm->data_count);
            printf("Final buffer: %s\n", sm->buffer);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            printf("Error state. Use RESET to continue.\n");
            return STATE_ERROR;
    }
}

State process_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            sm->current_state = handle_start_state(sm, event, data);
            break;
        case STATE_READING:
            sm->current_state = handle_reading_state(sm, event, data);
            break;
        case STATE_PROCESSING:
            sm->current_state = handle_processing_state(sm, event, data);
            break;
        case STATE_FINISHED:
            sm->current_state = handle_finished_state(sm, event, data);
            break;
        case STATE_ERROR:
            sm->current_state = handle_error_state(sm, event, data);
            break;
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
    
    return sm->current