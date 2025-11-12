//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u/%u\n", sm->data_count, sm->max_data);
    printf("Buffer Position: %u\n", sm->buffer_pos);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_DATA_READY, "Sample data 2", 13},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_RESET, "", 0}
    };
    
    uint32_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (uint32_t i = 0; i < num_events; i++) {
        printf("Processing event %u\n", i);
        if (state_machine_handle_event(&sm, &events[i]) == 0) {
            print_state_info(&sm);
        } else {
            printf("Event handling failed