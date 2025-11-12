//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_len;
};

struct EventData {
    enum Event event_type;
    char data[128];
    uint32_t data_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int process_data(struct StateMachine *sm, const char *data, uint32_t data_len) {
    if (sm == NULL || data == NULL || data_len == 0) return 0;
    if (data_len > sizeof(sm->buffer) - sm->buffer_len - 1) return 0;
    
    memcpy(sm->buffer + sm->buffer_len, data, data_len);
    sm->buffer_len += data_len;
    sm->buffer[sm->buffer_len] = '\0';
    
    return 1;
}

enum State handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) {
        return STATE_ERROR;
    }

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                state_machine_init(sm);
                sm->current_state = STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (process_data(sm, event_data->data, event_data->data_len)) {
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_SUCCESS) {
                sm->data_count++;
                if (sm->data_count >= sm->max_data) {
                    sm->current_state = STATE_COMPLETE;
                } else {
                    sm->current_state = STATE_READY;
                }
            } else if (event_data->event_type == EVENT_FAILURE) {
                sm->current_state = STATE_ERROR;
            } else if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_COMPLETE:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
    }
    
    return sm->current_state;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS: return "PROCESS";
        case EVENT_SUCCESS: return "SUCCESS";
        case EVENT_FAILURE: return "FAILURE";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_SUCCESS, "", 0},
        {EVENT_DATA_READY, "Sample data 2", 13},
        {EVENT_SUCCESS, "", 0},
        {EVENT_DATA_READY, "Invalid data that is too long and might cause buffer overflow if not handled properly", 100},
        {EVENT_FAILURE, "", 0},
        {EVENT_RESET, "", 0},
        {EVENT_DATA_READY, "Final data", 10},
        {EVENT_SUCCESS, "", 0}
    };
    
    uint32_t num_events = sizeof(events) / sizeof(events[0]);