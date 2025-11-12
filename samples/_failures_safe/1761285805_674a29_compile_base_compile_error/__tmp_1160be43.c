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
    uint32_t processed_count;
    uint32_t written_count;
    char buffer[256];
    uint8_t buffer_index;
};

struct EventData {
    enum Event event_type;
    char data[128];
    uint8_t data_length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->written_count = 0;
    sm->buffer_index = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_length >= sizeof(event_data->data)) return 0;
    return 1;
}

int process_data(struct StateMachine *sm, const char *data, uint8_t length) {
    if (sm == NULL || data == NULL || length == 0) return 0;
    if (sm->buffer_index + length >= sizeof(sm->buffer)) return 0;
    
    for (uint8_t i = 0; i < length; i++) {
        sm->buffer[sm->buffer_index++] = toupper(data[i]);
    }
    sm->processed_count++;
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->buffer_index == 0) return 0;
    
    for (uint8_t i = 0; i < sm->buffer_index; i++) {
        putchar(sm->buffer[i]);
    }
    putchar('\n');
    sm->written_count += sm->buffer_index;
    sm->buffer_index = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

enum State handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) {
        return STATE_ERROR;
    }

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (process_data(sm, event_data->data, event_data->data_length)) {
                    sm->data_count++;
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
                sm->current_state = STATE_WRITING;
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event_data->event_type == EVENT_WRITE_COMPLETE) {
                if (write_data(sm)) {
                    sm->current_state = STATE_DONE;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                sm->current_state = STATE_IDLE;
            }
            break;
            
        case STATE_DONE:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                sm->current_state = STATE_IDLE;
            }
            break;
    }
    
    return sm->current_state;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "hello world", 11},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_RESET, "", 0},
        {EVENT_DATA_READY, "test data", 9},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        enum State new_state = handle_event(&sm, &events[i]);