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
    sm->max_data = 10;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READING;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        return 1;
    }
    return 0;
}

int handle_reading_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_DATA_READY) {
        if (sm->buffer_pos + event_data->data_len < sizeof(sm->buffer)) {
            memcpy(sm->buffer + sm->buffer_pos, event_data->data, event_data->data_len);
            sm->buffer_pos += event_data->data_len;
            sm->current_state = STATE_PROCESSING;
            return 1;
        } else {
            sm->current_state = STATE_ERROR;
            return 0;
        }
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    return 0;
}

int handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
        if (sm->data_count < sm->max_data) {
            sm->data_count++;
            sm->current_state = STATE_WRITING;
            return 1;
        } else {
            sm->current_state = STATE_DONE;
            return 1;
        }
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    return 0;
}

int handle_writing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_WRITE_COMPLETE) {
        sm->current_state = STATE_READING;
        sm->buffer_pos = 0;
        return 1;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    return 0;
}

int handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_RESET) {
        state_machine_init(sm);
        return 1;
    }
    return 0;
}

int handle_done_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_RESET) {
        state_machine_init(sm);
        return 1;
    }
    return 0;
}

int process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, event_data);
        case STATE_READING:
            return handle_reading_state(sm, event_data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event_data);
        case STATE_WRITING:
            return handle_writing_state(sm, event_data);
        case STATE_ERROR:
            return handle_error_state(sm, event_data);
        case STATE_DONE:
            return handle_done_state(sm, event_data);
        default:
            return 0;
    }
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "