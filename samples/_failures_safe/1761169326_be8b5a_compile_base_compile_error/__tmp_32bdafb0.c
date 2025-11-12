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
    sm->max_data = 100;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '.' || c == ',';
}

enum State handle_idle_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_START:
            sm->data_count = 0;
            sm->buffer_pos = 0;
            sm->processed_count = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

enum State handle_reading_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_DATA_READY:
            if (sm->data_count >= sm->max_data) {
                return STATE_ERROR;
            }
            if (sm->buffer_pos >= sizeof(sm->buffer) - 1) {
                return STATE_ERROR;
            }
            if (!is_valid_char(event->data_char)) {
                return STATE_ERROR;
            }
            sm->buffer[sm->buffer_pos++] = event->data_char;
            sm->data_count++;
            if (sm->buffer_pos >= sizeof(sm->buffer) - 1 || sm->data_count >= sm->max_data) {
                return STATE_PROCESSING;
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

enum State handle_processing_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_PROCESS_COMPLETE:
            for (uint32_t i = 0; i < sm->buffer_pos; i++) {
                sm->buffer[i] = (char)toupper((unsigned char)sm->buffer[i]);
            }
            sm->processed_count = sm->buffer_pos;
            return STATE_WRITING;
        case EVENT_RESET:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

enum State handle_writing_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_WRITE_COMPLETE:
            for (uint32_t i = 0; i < sm->processed_count; i++) {
                putchar(sm->buffer[i]);
            }
            putchar('\n');
            if (sm->data_count >= sm->max_data) {
                return STATE_DONE;
            }
            sm->buffer_pos = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

enum State handle_error_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

enum State handle_done_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

void process_event(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return;
    
    enum State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(sm, event);
            break;
        case STATE_READING:
            new_state = handle_reading_state(sm, event);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state