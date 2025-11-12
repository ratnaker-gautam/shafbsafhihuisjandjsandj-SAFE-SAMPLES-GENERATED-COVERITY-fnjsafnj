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

void initialize_state_machine(StateMachine *sm) {
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
                    printf("Data received: %s\n", data);
                    return STATE_READING;
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            if (sm->data_count > 0) {
                printf("Reading complete. Total data items: %d\n", sm->data_count);
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing data: %s\n", sm->buffer);
            printf("Data processed successfully.\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_state_machine(sm);
            return STATE_START;
        default:
            printf("Error state. Use reset to continue.\n");
            return STATE_ERROR;
    }
}

void process_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return;
    
    State new_state;
    switch (sm->current_state) {
        case STATE_START:
            new_state = handle_start_state(sm, event, data);
            break;
        case STATE_READING:
            new_state = handle_reading_state(sm, event, data);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(sm, event, data);
            break;
        case STATE_FINISHED:
            new_state = handle_finished_state(sm, event, data);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(sm, event, data);
            break;
        default:
            new_state = STATE_ERROR;
            break;
    }
    
    if (new_state != sm->current_state) {
        printf("State transition: %d -> %d\n", sm->current_state, new_state);
        sm->current_state = new_state;
    }
}

int