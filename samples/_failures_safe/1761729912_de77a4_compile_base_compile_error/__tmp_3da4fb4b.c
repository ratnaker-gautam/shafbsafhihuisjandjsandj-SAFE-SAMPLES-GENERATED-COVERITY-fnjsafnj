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

int validate_data(const char *data) {
    if (data == NULL) return 0;
    
    size_t len = strlen(data);
    if (len == 0 || len >= 10) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (data[i] < '0' || data[i] > '9') return 0;
    }
    
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
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                const char *data_start = "D:123";
                size_t data_len = 3;
                
                if (validate_data(data_start + 2)) {
                    if (sm->buffer_len + data_len < MAX_INPUT_LEN) {
                        memcpy(sm->buffer + sm->buffer_len, data_start + 2, data_len);
                        sm->buffer_len += data_len;
                        sm->data_count++;
                        printf("Data received: %.*s\n", (int)data_len, data_start + 2);
                        
                        if (sm->data_count >= 3) {
                            return STATE_PROCESSING;
                        }
                        return STATE_READING;
                    }
                }
            }
            return STATE_ERROR;
        case EVENT_END:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_END:
            printf("Processing %d data items: %s\n", sm->data_count, sm->buffer);
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_END:
            printf("State machine finished successfully\n");
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("State machine entered error state\n");
    return STATE_ERROR;
}

void process_state_machine(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    size_t pos = 0;
    Event event;
    
    while (sm->current_state != STATE_FINISHED && sm->current_state != STATE_ERROR) {
        event = get_next_event(input, &pos);
        
        switch (sm->current_state) {
            case STATE_START:
                sm->current_state = handle_start_state(sm, event);
                break;
            case STATE_READING:
                sm->current_state = handle_reading_state(sm, event);
                break;
            case STATE_PROCESSING:
                sm->current_state = handle_processing_state(sm, event);
                break;
            case STATE_FINISHED:
                sm->current_state = handle_finished_state(sm, event);
                break;
            case STATE_ERROR:
                sm->current_state = handle_error_state(sm, event);
                break;
        }
        
        if (event == EVENT_END) break;
        if (input[pos] != '\0') {