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

int state_machine_validate_event(const struct EventData *event) {
    if (event == NULL) return 0;
    if (event->event_type < EVENT_START || event->event_type > EVENT_RESET) return 0;
    if (event->data_len >= sizeof(event->data)) return 0;
    return 1;
}

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || !state_machine_validate_event(event)) {
        return 0;
    }

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                sm->buffer_pos = 0;
            }
            break;
            
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (event->data_len > 0 && sm->buffer_pos + event->data_len < sizeof(sm->buffer)) {
                    memcpy(sm->buffer + sm->buffer_pos, event->data, event->data_len);
                    sm->buffer_pos += event->data_len;
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                if (sm->data_count < sm->max_data) {
                    sm->data_count++;
                    sm->current_state = STATE_WRITING;
                } else {
                    sm->current_state = STATE_DONE;
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                sm->current_state = STATE_READING;
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_DONE:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
    }
    
    return 1;
}

void state_machine_print_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    
    printf("State: %s, Data Count: %u/%u, Buffer: %u bytes\n",
           state_names[sm->current_state],
           sm->data_count,
           sm->max_data,
           sm->buffer_pos);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_DATA_READY, "More sample data", 15},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_DATA_READY, "Final data", 10},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_ERROR, "Simulated error", 15},
        {EVENT_RESET, "", 0},
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "New session data", 16},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0}
    };
    
    uint32_t num_events = sizeof(events) / sizeof(events[0]);