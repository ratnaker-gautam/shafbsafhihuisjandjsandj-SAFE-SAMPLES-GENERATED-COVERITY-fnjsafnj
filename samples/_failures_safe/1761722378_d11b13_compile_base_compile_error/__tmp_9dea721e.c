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
    char data[64];
    uint32_t data_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int process_buffer(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        sm->buffer[i] = toupper((unsigned char)sm->buffer[i]);
    }
    sm->processed_count++;
    return 1;
}

enum State handle_state_transition(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data && event_data->data_len > 0) {
                    size_t copy_len = event_data->data_len;
                    if (copy_len > sizeof(sm->buffer) - sm->buffer_pos - 1) {
                        copy_len = sizeof(sm->buffer) - sm->buffer_pos - 1;
                    }
                    memcpy(sm->buffer + sm->buffer_pos, event_data->data, copy_len);
                    sm->buffer_pos += copy_len;
                    sm->data_count++;
                    return STATE_PROCESSING;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
                if (process_buffer(sm)) {
                    return STATE_WRITING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event_data->event_type == EVENT_WRITE_COMPLETE) {
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                if (sm->data_count >= sm->max_data) {
                    return STATE_DONE;
                } else {
                    return STATE_READING;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return STATE_IDLE;
            }
            break;
            
        case STATE_DONE:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return STATE_IDLE;
            }
            break;
    }
    
    return sm->current_state;
}

void execute_state_action(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_READING:
            printf("Reading data... (%u/%u)\n", sm->data_count + 1, sm->max_data);
            break;
            
        case STATE_PROCESSING:
            printf("Processing buffer: %s\n", sm->buffer);
            break;
            
        case STATE_WRITING:
            printf("Writing result: %s\n", sm->buffer);
            break;
            
        case STATE_ERROR:
            printf("Error state encountered!\n");
            break;
            
        case STATE_DONE:
            printf("Processing complete! Processed %u items.\n", sm->processed_count);
            break;
            
        default:
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "hello", 5},
        {EVENT_PROCESS_COMPLETE, "", 0