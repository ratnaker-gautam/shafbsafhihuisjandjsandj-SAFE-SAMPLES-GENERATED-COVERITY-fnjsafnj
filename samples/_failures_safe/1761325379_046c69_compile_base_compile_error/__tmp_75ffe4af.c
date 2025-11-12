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

void initialize_state_machine(StateMachine *sm) {
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

int validate_data(const char *data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_LEN - 1) return 0;
    
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
                sm->buffer[sm->buffer_len++] = 'D';
                sm->buffer[sm->buffer_len++] = ':';
                sm->data_count++;
                printf("Data received, count: %d\n", sm->data_count);
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
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
            if (sm->data_count < 5) {
                return STATE_READING;
            } else {
                printf("Maximum data count reached\n");
                return STATE_FINISHED;
            }
        case EVENT_END:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("Processing complete. Total data items: %d\n", sm->data_count);
    return STATE_FINISHED;
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("Error state reached\n");
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

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    const char *test_input = "S D:123 D:456 D:789 D:012 D:345";
    size_t pos = 0;
    
    while (sm.current_state != STATE_FINISHED && sm.current_state != STATE_ERROR) {
        Event event = get_next_event(test_input, &pos);
        
        if (event == EVENT_DATA) {
            size_t data_start = pos;
            size_t data_len = 0;
            
            while (test_input[pos] >= '0' && test_input[pos] <= '9') {
                data_len++;
                pos++;
                if (data_len >= 3) break;
            }
            
            if (!validate_data(test_input + data_start, data_len)) {
                event = EVENT_INVALID;
            }
        }