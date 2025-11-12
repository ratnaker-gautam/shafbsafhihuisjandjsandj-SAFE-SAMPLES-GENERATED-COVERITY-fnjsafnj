//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_processed;
} StateMachine;

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event evt);
State handle_reading(StateMachine* sm, Event evt);
State handle_processing(StateMachine* sm, Event evt);
State handle_writing(StateMachine* sm, Event evt);
State handle_error(StateMachine* sm, Event evt);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

State handle_idle(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            sm->buffer_pos = 0;
            sm->data_processed = 0;
            printf("Starting data processing...\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA_READY:
            if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos] = 'A' + (sm->buffer_pos % 26);
                sm->buffer_pos++;
                printf("Read data byte %zu\n", sm->buffer_pos);
                
                if (sm->buffer_pos >= 10) {
                    sm->buffer[sm->buffer_pos] = '\0';
                    return STATE_PROCESSING;
                }
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->buffer_pos > 0 && sm->buffer_pos < sizeof(sm->buffer)) {
                for (size_t i = 0; i < sm->buffer_pos; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                sm->data_processed = 1;
                printf("Data processed: %s\n", sm->buffer);
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            if (sm->data_processed) {
                printf("Data written successfully\n");
                return STATE_IDLE;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            sm->buffer_pos = 0;
            sm->data_processed = 0;
            printf("State machine reset\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

void process_event(StateMachine* sm, Event evt) {
    if (sm == NULL || evt < 0 || evt >= EVENT_COUNT) return;
    
    StateHandler handler = state_handlers[sm->current_state];
    if (handler == NULL) return;
    
    State new_state = handler(sm, evt);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        sm->current_state = new_state;
    }
}

int main() {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_pos = 0;
    sm.data_processed = 0;
    
    Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_RESET
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);