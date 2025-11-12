//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
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
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_START:
            printf("State transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

enum State handle_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_DATA_READY:
            if (event_data->data_len > 0 && event_data->data_len < sizeof(sm->buffer)) {
                strncpy(sm->buffer, event_data->data, event_data->data_len);
                sm->buffer[event_data->data_len] = '\0';
                printf("Data loaded: %s\n", sm->buffer);
                return STATE_PROCESSING;
            } else {
                printf("Invalid data length\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("State transition: READY -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

enum State handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_SUCCESS:
            if (sm->data_count < sm->max_data) {
                sm->data_count++;
                printf("Processing successful. Data count: %u\n", sm->data_count);
                return STATE_COMPLETE;
            } else {
                printf("Maximum data count reached\n");
                return STATE_ERROR;
            }
        case EVENT_FAILURE:
            printf("Processing failed\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("State transition: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

enum State handle_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_RESET:
            printf("State transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        case EVENT_START:
            printf("State transition: COMPLETE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

enum State handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_RESET:
            printf("State transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

void process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) {
        printf("Invalid state machine or event data\n");
        return;
    }
    
    enum State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(sm, event_data);
            break;
        case STATE_READY:
            new_state = handle_ready_state(sm, event_data);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(sm, event_data);
            break;
        case STATE_COMPLETE:
            new_state = handle_complete_state(sm, event_data);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(sm, event_data);
            break;
        default:
            new_state = STATE_ERROR;
            break;
    }
    
    sm