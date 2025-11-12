//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
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
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int process_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READING;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        return 1;
    }
    return 0;
}

int process_reading_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_DATA_READY) {
        if (sm->buffer_pos + event_data->data_len < sizeof(sm->buffer)) {
            memcpy(sm->buffer + sm->buffer_pos, event_data->data, event_data->data_len);
            sm->buffer_pos += event_data->data_len;
            sm->data_count++;
            if (sm->data_count >= sm->max_data) {
                sm->current_state = STATE_PROCESSING;
            }
            return 1;
        }
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 1;
    }
    return 0;
}

int process_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
        sm->current_state = STATE_WRITING;
        return 1;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 1;
    }
    return 0;
}

int process_writing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_WRITE_COMPLETE) {
        sm->current_state = STATE_DONE;
        return 1;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 1;
    }
    return 0;
}

int process_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return 1;
    }
    return 0;
}

int process_done_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return 1;
    }
    return 0;
}

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return process_idle_state(sm, event_data);
        case STATE_READING:
            return process_reading_state(sm, event_data);
        case STATE_PROCESSING:
            return process_processing_state(sm, event_data);
        case STATE_WRITING:
            return process_writing_state(sm, event_data);
        case STATE_ERROR:
            return process_error_state(sm, event_data);
        case STATE_DONE:
            return process_done_state(sm, event_data);
        default:
            return 0;
    }
}

void print_state_machine_status(const struct State