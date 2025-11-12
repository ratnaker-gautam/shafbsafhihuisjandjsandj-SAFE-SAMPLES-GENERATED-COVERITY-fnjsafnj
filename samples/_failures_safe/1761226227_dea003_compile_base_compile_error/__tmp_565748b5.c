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

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ';
}

State handle_start_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL || data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                size_t data_len = strlen(data);
                if (data_len > 0 && data_len < MAX_INPUT_LEN - sm->buffer_len) {
                    for (size_t i = 0; i < data_len; i++) {
                        if (!is_valid_input_char(data[i])) {
                            return STATE_ERROR;
                        }
                    }
                    strncat(sm->buffer, data, MAX_INPUT_LEN - sm->buffer_len - 1);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            return handle_reading_state(sm, event, data);
        case EVENT_COMPLETE:
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
            return STATE_ERROR;
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

State state_machine_transition(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event, data);
        case STATE_READING:
            return handle_reading_state(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event, data);
        case STATE_FINISHED:
            return handle_finished_state(sm, event, data);
        case STATE_ERROR:
            return handle_error_state(sm, event, data);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(const StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_START: printf("START"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_FINISHED: printf("FINISHED"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\nBuffer: %s\n", sm->buffer);
    printf("Data count: %d\n", sm->data_count);
    printf("Buffer length: %zu\n", sm->buffer_len);
}

int get_user_event(Event *event, char *input_buffer, size_t buffer_size) {
    if (event == NULL || input_buffer == NULL || buffer_size == 0) return 0;
    
    printf("\nAvailable events:\n");
    printf("1 - START\n");
    printf("2 - DATA\n");
    printf("3 - COMPLETE\n");
    printf("4 - RESET\n");
    printf("5 - EXIT\n");
    printf("Choose event (1-5): ");