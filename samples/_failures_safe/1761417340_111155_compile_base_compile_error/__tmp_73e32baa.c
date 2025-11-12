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

int safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) return 0;
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

void handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READING;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
    }
}

void handle_reading_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    if (event_data->event_type == EVENT_DATA_READY) {
        if (sm->buffer_pos + event_data->data_len < sizeof(sm->buffer)) {
            memcpy(sm->buffer + sm->buffer_pos, event_data->data, event_data->data_len);
            sm->buffer_pos += event_data->data_len;
            sm->data_count++;
            if (sm->data_count >= sm->max_data) {
                sm->current_state = STATE_PROCESSING;
            }
        } else {
            sm->current_state = STATE_ERROR;
        }
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
    }
}

void handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
        for (uint32_t i = 0; i < sm->buffer_pos; i++) {
            sm->buffer[i] = toupper(sm->buffer[i]);
        }
        sm->current_state = STATE_WRITING;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
    }
}

void handle_writing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    if (event_data->event_type == EVENT_WRITE_COMPLETE) {
        printf("Processed data: %s\n", sm->buffer);
        sm->current_state = STATE_DONE;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
    }
}

void handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
    }
}

void handle_done_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
    }
}

void process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return;
    switch (sm->current_state) {
        case STATE_IDLE:
            handle_idle_state(sm, event_data);
            break;
        case STATE_READING:
            handle_reading_state(sm, event_data);
            break;