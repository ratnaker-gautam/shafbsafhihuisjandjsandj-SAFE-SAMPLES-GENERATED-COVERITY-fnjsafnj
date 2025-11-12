//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    char data[64];
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

int safe_string_copy(char *dest, size_t dest_size, const char *src, size_t src_len) {
    if (dest == NULL || src == NULL) return 0;
    if (dest_size == 0) return 0;
    if (src_len >= dest_size) return 0;
    
    for (size_t i = 0; i < src_len; i++) {
        if (src[i] == '\0') break;
        dest[i] = src[i];
    }
    dest[src_len] = '\0';
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_START:
            sm->data_count = 0;
            sm->buffer_pos = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

enum State handle_reading_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_DATA_READY:
            if (sm->data_count >= sm->max_data) return STATE_ERROR;
            if (event_data->data_len > 0) {
                size_t remaining = sizeof(sm->buffer) - sm->buffer_pos - 1;
                if (event_data->data_len > remaining) return STATE_ERROR;
                
                if (!safe_string_copy(sm->buffer + sm->buffer_pos, remaining + 1, 
                                    event_data->data, event_data->data_len)) {
                    return STATE_ERROR;
                }
                sm->buffer_pos += event_data->data_len;
                sm->data_count++;
            }
            return STATE_PROCESSING;
        case EVENT_RESET:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

enum State handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_PROCESS_COMPLETE:
            for (size_t i = 0; i < sm->buffer_pos && i < sizeof(sm->buffer); i++) {
                if (sm->buffer[i] != '\0') {
                    sm->buffer[i] = (char)toupper((unsigned char)sm->buffer[i]);
                }
            }
            return STATE_WRITING;
        case EVENT_RESET:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

enum State handle_writing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_WRITE_COMPLETE:
            if (sm->data_count >= sm->max_data) {
                return STATE_DONE;
            }
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

enum State handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    if (event_data->event_type == EVENT_RESET) {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum State handle_done_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    if (event_data->event_type == EVENT_RESET) {
        return