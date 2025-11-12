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

int validate_event_data(const struct EventData *event) {
    if (event == NULL) return 0;
    if (event->event_type < EVENT_START || event->event_type > EVENT_RESET) return 0;
    if (event->data_len >= sizeof(event->data)) return 0;
    return 1;
}

int process_data(struct StateMachine *sm, const char *data, uint32_t len) {
    if (sm == NULL || data == NULL || len == 0) return 0;
    if (sm->buffer_pos + len >= sizeof(sm->buffer)) return 0;
    
    for (uint32_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)data[i]) && data[i] != ' ') return 0;
        sm->buffer[sm->buffer_pos++] = data[i];
    }
    sm->buffer[sm->buffer_pos] = '\0';
    return 1;
}

int write_data(const struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

enum State handle_event(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || !validate_event_data(event)) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (process_data(sm, event->data, event->data_len)) {
                    sm->data_count++;
                    if (sm->data_count >= sm->max_data) {
                        return STATE_PROCESSING;
                    }
                    return STATE_READING;
                }
                return STATE_ERROR;
            } else if (event->event_type == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                return STATE_WRITING;
            } else if (event->event_type == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                if (write_data(sm)) {
                    return STATE_DONE;
                }
                return STATE_ERROR;
            } else if (event->event_type == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return STATE_IDLE;
            }
            break;
            
        case STATE_DONE:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return STATE_IDLE;
            }
            break;
            
        default:
            return STATE_ERROR;
    }
    
    return sm->current_state;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Hello", 5},
        {EVENT_DATA_READY, "World", 5},
        {EVENT_DATA_READY, "Test123", 7},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_RESET, "", 0}
    };
    
    uint32_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (uint32_t i = 0; i < num_events; i++) {
        enum State new_state = handle_event(&sm, &events[i]);
        sm.current_state = new_state;
        
        printf("Event %u: State transition to %d\n", i, new_state);
        
        if (sm.current_state == STATE_ERROR) {
            printf("Error occurred, waiting for reset\n");
            break;
        }
    }
    
    return 0;
}