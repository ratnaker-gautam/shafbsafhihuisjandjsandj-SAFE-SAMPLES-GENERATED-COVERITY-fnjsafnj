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
    char data;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
            
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = event->data;
                    sm->data_count++;
                }
                if (sm->data_count >= 10) {
                    sm->current_state = STATE_PROCESSING;
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                for (uint32_t i = 0; i < sm->buffer_pos; i++) {
                    if (isalpha(sm->buffer[i])) {
                        sm->buffer[i] = toupper(sm->buffer[i]);
                    }
                }
                sm->processed_count = sm->buffer_pos;
                sm->current_state = STATE_WRITING;
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                printf("Processed data: %s\n", sm->buffer);
                sm->current_state = STATE_DONE;
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
    
    return 0;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, 0},
        {EVENT_DATA_READY, 'h'},
        {EVENT_DATA_READY, 'e'},
        {EVENT_DATA_READY, 'l'},
        {EVENT_DATA_READY, 'l'},
        {EVENT_DATA_READY, 'o'},
        {EVENT_DATA_READY, ' '},
        {EVENT_DATA_READY, 'w'},
        {EVENT_DATA_READY, 'o'},
        {EVENT_DATA_READY, 'r'},
        {EVENT_DATA_READY, 'l'},
        {EVENT_DATA_READY, 'd'},
        {EVENT_PROCESS_COMPLETE, 0},
        {EVENT_WRITE_COMPLETE, 0},
        {EVENT_RESET, 0}
    };
    
    uint32_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (uint32_t i = 0; i < num_events; i++) {
        if (state_machine_handle_event(&sm, &events[i]) != 0) {
            fprintf(stderr, "Error handling event\n");
            return 1;
        }
    }
    
    return 0;
}