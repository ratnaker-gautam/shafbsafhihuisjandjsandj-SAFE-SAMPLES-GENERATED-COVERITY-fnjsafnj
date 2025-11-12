//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
            if (event_data->data_len > 0 && event_data->data_len < sizeof(sm->buffer)) {
                strncpy(sm->buffer, event_data->data, event_data->data_len);
                sm->buffer[event_data->data_len] = '\0';
                sm->data_count = 1;
                return STATE_READY;
            }
            break;
        default:
            break;
    }
    return STATE_IDLE;
}

enum State handle_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_DATA_READY:
            if (sm->data_count < sm->max_data && event_data->data_len > 0) {
                size_t current_len = strlen(sm->buffer);
                size_t remaining = sizeof(sm->buffer) - current_len - 1;
                if (event_data->data_len < remaining) {
                    strncat(sm->buffer, event_data->data, event_data->data_len);
                    sm->data_count++;
                }
            }
            return STATE_READY;
        case EVENT_PROCESS:
            return STATE_PROCESSING;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        default:
            break;
    }
    return STATE_READY;
}

enum State handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_SUCCESS:
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            return STATE_ERROR;
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        default:
            break;
    }
    return STATE_PROCESSING;
}

enum State handle_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        default:
            break;
    }
    return STATE_COMPLETE;
}

enum State handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_RESET:
            state_machine_init(sm);
            return STATE_IDLE;
        default:
            break;
    }
    return STATE_ERROR;
}

enum State process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, event_data);
        case STATE_READY:
            return handle_ready_state(sm, event_data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event_data);
        case STATE_COMPLETE:
            return handle_complete_state(sm, event_data);
        case STATE_ERROR:
            return handle_error_state(sm, event_data);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u\n", sm->data_count);
    printf("Buffer: %s\n", sm->buffer);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init