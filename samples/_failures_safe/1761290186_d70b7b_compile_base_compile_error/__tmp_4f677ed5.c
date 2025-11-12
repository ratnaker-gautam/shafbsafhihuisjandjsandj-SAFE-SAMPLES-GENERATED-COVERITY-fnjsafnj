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
    return isprint(c) && c != '\0';
}

enum State handle_idle_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

enum State handle_reading_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_DATA_READY:
            if (is_valid_char(event->data_char) && sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos++] = event->data_char;
                sm->data_count++;
                if (sm->data_count >= sm->max_data) {
                    return STATE_PROCESSING;
                }
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

enum State handle_processing_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->buffer_pos > 0) {
                for (uint32_t i = 0; i < sm->buffer_pos; i++) {
                    if (is_valid_char(sm->buffer[i])) {
                        sm->buffer[i] = toupper(sm->buffer[i]);
                    }
                }
                sm->processed_count = sm->buffer_pos;
                return STATE_WRITING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

enum State handle_writing_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->event_type) {
        case EVENT_WRITE_COMPLETE:
            if (sm->processed_count > 0) {
                for (uint32_t i = 0; i < sm->processed_count; i++) {
                    if (is_valid_char(sm->buffer[i])) {
                        putchar(sm->buffer[i]);
                    }
                }
                putchar('\n');
                return STATE_DONE;
            } else {
                return STATE_ERROR;
            }
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

enum State handle_error_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    if (event->event_type == EVENT_RESET) {
        state_machine_init(sm);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum State handle_done_state(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    
    if (event->event_type == EVENT_RESET) {
        state_machine_init(sm);
        return STATE_IDLE;
    }
    return STATE_DONE;
}

void process_event(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return;
    
    enum State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            next_state = handle_idle_state(sm, event);
            break;
        case STATE_READING:
            next_state = handle_reading_state(sm, event);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing_state(sm, event);
            break;
        case STATE_WRITING:
            next_state = handle_writing_state(sm, event);
            break