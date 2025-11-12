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
    sm->max_data = 100;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int process_data(struct StateMachine *sm, const char *data, uint32_t len) {
    if (sm == NULL || data == NULL || len == 0) return 0;
    if (len > sizeof(sm->buffer) - sm->buffer_pos - 1) return 0;
    
    for (uint32_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)data[i]) && data[i] != ' ') return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_pos, data, len);
    sm->buffer_pos += len;
    sm->buffer[sm->buffer_pos] = '\0';
    return 1;
}

int write_data(const struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    printf("Output: %s\n", sm->buffer);
    return 1;
}

enum State handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                state_machine_init(sm);
                sm->current_state = STATE_READING;
                printf("State: IDLE -> READING\n");
            }
            break;
            
        case STATE_READING:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (process_data(sm, event_data->data, event_data->data_len)) {
                    sm->data_count++;
                    if (sm->data_count >= sm->max_data) {
                        sm->current_state = STATE_PROCESSING;
                        printf("State: READING -> PROCESSING\n");
                    }
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("State: READING -> ERROR\n");
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                printf("State: READING -> ERROR\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
                sm->current_state = STATE_WRITING;
                printf("State: PROCESSING -> WRITING\n");
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                printf("State: PROCESSING -> ERROR\n");
            }
            break;
            
        case STATE_WRITING:
            if (event_data->event_type == EVENT_WRITE_COMPLETE) {
                if (write_data(sm)) {
                    sm->current_state = STATE_DONE;
                    printf("State: WRITING -> DONE\n");
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("State: WRITING -> ERROR\n");
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                printf("State: WRITING -> ERROR\n");
            }
            break;
            
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                sm->current_state = STATE_IDLE;
                printf("State: ERROR -> IDLE\n");
            }
            break;
            
        case STATE_DONE:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                sm->current_state = STATE_IDLE;
                printf("State: DONE -> IDLE\n");
            }
            break;
            
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
    
    return sm->current_state;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Hello ", 6},
        {EVENT_DATA