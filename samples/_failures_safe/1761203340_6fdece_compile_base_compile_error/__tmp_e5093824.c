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

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                return 0;
            }
            break;
            
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (event->data_len > 0 && event->data_len < sizeof(sm->buffer) - sm->buffer_pos) {
                    memcpy(sm->buffer + sm->buffer_pos, event->data, event->data_len);
                    sm->buffer_pos += event->data_len;
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                return -1;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                if (sm->buffer_pos > 0) {
                    sm->current_state = STATE_WRITING;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                return -1;
            }
            break;
            
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                sm->data_count++;
                if (sm->data_count >= sm->max_data) {
                    sm->current_state = STATE_DONE;
                } else {
                    sm->current_state = STATE_READING;
                    sm->buffer_pos = 0;
                    memset(sm->buffer, 0, sizeof(sm->buffer));
                }
                return 0;
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                return -1;
            }
            break;
            
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_DONE:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return 0;
            }
            break;
    }
    
    return -1;
}

void print_state(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    
    printf("Current state: %s, Data count: %u/%u\n", 
           state_names[sm->current_state], sm->data_count, sm->max_data);
}

int main(void) {
    struct StateMachine sm;
    struct EventData event;
    
    state_machine_init(&sm);
    
    printf("State Machine Simulation\n");
    printf("========================\n");
    
    print_state(&sm);
    
    event.event_type = EVENT_START;
    event.data_len = 0;
    if (state_machine_handle_event(&sm, &event) == 0) {
        printf("Event: START -> ");
        print_state(&sm);
    }
    
    event.event_type = EVENT_DATA_READY;
    strncpy(event.data, "Sample data", sizeof(event.data) - 1);
    event.data[sizeof(event.data) - 1] = '\0';
    event.data_len = strlen(event.data);
    if (state_machine_handle_event(&sm, &event) == 0) {
        printf("Event: DATA_READY -> ");
        print_state(&sm);
    }
    
    event.event_type = EVENT_PROCESS_COMPLETE;
    event.data_len = 0;
    if (state_machine_handle_event(&sm, &event) == 0) {
        printf("Event: PROCESS_COMPLETE -> ");
        print_state(&sm);
    }
    
    event.event