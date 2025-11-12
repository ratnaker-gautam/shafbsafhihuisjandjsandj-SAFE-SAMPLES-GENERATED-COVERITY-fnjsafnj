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

State handle_start(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_finished(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine*, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            memset(sm->buffer, 0, MAX_INPUT_LEN);
            sm->buffer_len = 0;
            sm->data_count = 0;
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_len] = 'A';
                sm->buffer_len++;
                sm->data_count++;
                printf("Data received, count: %d\n", sm->data_count);
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            if (sm->data_count > 0) {
                printf("Processing %d data items\n", sm->data_count);
                printf("Buffer contents: %s\n", sm->buffer);
                return STATE_FINISHED;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            printf("Error state - reset required\n");
            return STATE_ERROR;
    }
}

int process_event(StateMachine *sm, Event event) {
    if (sm == NULL || sm->current_state >= sizeof(state_handlers)/sizeof(state_handlers[0])) {
        return -1;
    }
    
    State new_state = state_handlers[sm->current_state](sm, event);
    sm->current_state = new_state;
    return 0;
}

void simulate_state_machine(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    
    process_event(&sm, EVENT_START);
    
    for (int i = 0; i < 5; i++) {
        if (sm.current_state == STATE_READING) {
            process_event(&sm, EVENT_DATA);
        }
    }
    
    process_event(&sm, EVENT_COMPLETE);
    
    if (sm.current_state == STATE_PROCESSING) {
        process_event(&sm, EVENT_COMPLETE);
    }
    
    if (sm.current_state == STATE_FINISHED) {
        process_event(&sm, EVENT_RESET);
    }
    
    if (sm.current_state == STATE_START) {
        printf("State machine reset successfully\n");
    }
}

int main(void) {
    simulate_state_machine();
    return 0;
}