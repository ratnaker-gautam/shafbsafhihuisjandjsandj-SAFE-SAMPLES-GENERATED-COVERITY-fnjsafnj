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
    char buffer[256];
    uint32_t buffer_pos;
    uint32_t processed_count;
};

struct EventData {
    enum Event event_type;
    char data_char;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_event(enum Event evt) {
    return (evt >= EVENT_START && evt <= EVENT_RESET);
}

int is_valid_state(enum State st) {
    return (st >= STATE_IDLE && st <= STATE_DONE);
}

int handle_idle_state(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    
    if (evt_data->event_type == EVENT_START) {
        sm->current_state = STATE_READING;
        sm->data_count = 0;
        sm->buffer_pos = 0;
        sm->processed_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return 1;
    }
    return 0;
}

int handle_reading_state(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    
    if (evt_data->event_type == EVENT_DATA_READY) {
        if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
            sm->buffer[sm->buffer_pos++] = evt_data->data_char;
            sm->data_count++;
            if (sm->data_count >= 10) {
                sm->current_state = STATE_PROCESSING;
            }
            return 1;
        } else {
            sm->current_state = STATE_ERROR;
            return 0;
        }
    } else if (evt_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    return 0;
}

int handle_processing_state(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    
    if (evt_data->event_type == EVENT_PROCESS_COMPLETE) {
        for (uint32_t i = 0; i < sm->buffer_pos; i++) {
            if (isalnum((unsigned char)sm->buffer[i])) {
                sm->processed_count++;
            }
        }
        sm->current_state = STATE_WRITING;
        return 1;
    } else if (evt_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    return 0;
}

int handle_writing_state(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    
    if (evt_data->event_type == EVENT_WRITE_COMPLETE) {
        printf("Processed %u alphanumeric characters from buffer: %s\n", 
               sm->processed_count, sm->buffer);
        sm->current_state = STATE_DONE;
        return 1;
    } else if (evt_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    return 0;
}

int handle_error_state(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    
    if (evt_data->event_type == EVENT_RESET) {
        state_machine_init(sm);
        return 1;
    }
    return 0;
}

int handle_done_state(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    
    if (evt_data->event_type == EVENT_RESET) {
        state_machine_init(sm);
        return 1;
    }
    return 0;
}

int process_event(struct StateMachine *sm, const struct EventData *evt_data) {
    if (sm == NULL || evt_data == NULL) return 0;
    if (!is_valid_state(sm->current_state) || !is_valid_event(evt_data->event_type)) {
        return 0;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, evt_data);
        case STATE_READING:
            return handle_reading_state(sm, evt_data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, evt_data);
        case STATE_WRITING:
            return handle_writing_state(s