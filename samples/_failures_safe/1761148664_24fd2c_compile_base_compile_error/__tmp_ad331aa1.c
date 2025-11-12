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
    size_t buffer_pos;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event, const char *data);
State handle_reading_state(StateMachine *sm, Event event, const char *data);
State handle_processing_state(StateMachine *sm, Event event, const char *data);
State handle_finished_state(StateMachine *sm, Event event, const char *data);
State handle_error_state(StateMachine *sm, Event event, const char *data);

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
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
                if (sm->buffer_pos + data_len < MAX_INPUT_LEN - 1) {
                    strncpy(sm->buffer + sm->buffer_pos, data, data_len);
                    sm->buffer_pos += data_len;
                    sm->data_count++;
                    printf("Data received: %s (Total: %d)\n", data, sm->data_count);
                    return STATE_READING;
                } else {
                    printf("Buffer full\n");
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Total data items: %d\n", sm->data_count);
            printf("Buffer contents: %s\n", sm->buffer);
            return STATE_FINISHED;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_START;
        default:
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
    
    return sm->current_state;
}

Event get_user_event() {
    char input[MAX_INPUT_LEN];
    
    printf("\nAvailable events:\n");
    printf("1 - START\n");
    printf("2 - DATA\n");
    printf("3 - COMPLETE