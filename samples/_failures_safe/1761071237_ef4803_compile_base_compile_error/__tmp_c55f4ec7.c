//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_END,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_finished_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

Event get_next_event(const char *input, size_t *pos) {
    if (input == NULL || pos == NULL) return EVENT_INVALID;
    
    size_t i = *pos;
    if (input[i] == '\0') return EVENT_END;
    
    if (i == 0 && input[i] == 'S') return EVENT_START;
    
    if (input[i] == 'D' && input[i + 1] == ':') {
        *pos += 2;
        return EVENT_DATA;
    }
    
    return EVENT_INVALID;
}

int validate_data(const char *input, size_t *pos, char *buffer, size_t buffer_size) {
    if (input == NULL || pos == NULL || buffer == NULL || buffer_size == 0) return 0;
    
    size_t start = *pos;
    size_t len = 0;
    
    while (input[start + len] != '\0' && input[start + len] != ';' && len < buffer_size - 1) {
        if (input[start + len] < 32 || input[start + len] > 126) return 0;
        len++;
    }
    
    if (len == 0) return 0;
    
    memcpy(buffer, &input[start], len);
    buffer[len] = '\0';
    *pos += len;
    
    return 1;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < 5) {
                sm->data_count++;
                printf("Processing data item %d\n", sm->data_count);
                return STATE_PROCESSING;
            } else {
                return STATE_FINISHED;
            }
        case EVENT_END:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
        case EVENT_END:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_END:
            printf("Processing complete. Total items: %d\n", sm->data_count);
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("Error state encountered\n");
    return STATE_ERROR;
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_FINISHED:
            return handle_finished_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

int run_state_machine(const char *input) {
    if (input == NULL) return 0;
    
    StateMachine sm;
    initialize_machine(&sm);
    
    size_t pos = 0;
    Event event;
    
    while (sm.current_state != STATE_FINISHED && sm.current_state != STATE_ERROR) {
        event = get_next_event(input, &pos);
        sm.current_state = process_event(&sm, event);
        
        if (event == EVENT_DATA) {
            if (!validate_data(input, &pos, sm.buffer, sizeof(sm.buffer))) {
                sm.current_state = STATE_ERROR;
            }
        }
        
        if (input[pos] == ';') pos++;
        if (