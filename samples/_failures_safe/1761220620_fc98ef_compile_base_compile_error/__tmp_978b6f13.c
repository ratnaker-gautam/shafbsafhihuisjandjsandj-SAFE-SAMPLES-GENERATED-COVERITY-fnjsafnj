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
    enum Event event;
    char data[128];
    size_t data_len;
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
    if (event_data->event < EVENT_START || event_data->event > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event == EVENT_START) {
        sm->current_state = STATE_READY;
        sm->data_count = 0;
        return 1;
    }
    return 0;
}

int handle_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event == EVENT_DATA_READY) {
        if (sm->data_count < sm->max_data) {
            size_t copy_len = event_data->data_len;
            if (copy_len > sizeof(sm->buffer) - 1) {
                copy_len = sizeof(sm->buffer) - 1;
            }
            memcpy(sm->buffer, event_data->data, copy_len);
            sm->buffer[copy_len] = '\0';
            sm->current_state = STATE_PROCESSING;
            return 1;
        } else {
            sm->current_state = STATE_ERROR;
            return 0;
        }
    } else if (event_data->event == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event == EVENT_SUCCESS) {
        sm->data_count++;
        sm->current_state = STATE_COMPLETE;
        return 1;
    } else if (event_data->event == EVENT_FAILURE) {
        sm->current_state = STATE_ERROR;
        return 0;
    } else if (event_data->event == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int handle_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        return 1;
    } else if (event_data->event == EVENT_START) {
        sm->current_state = STATE_READY;
        return 1;
    }
    return 0;
}

int handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
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
            return 0;
    }
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u/%u\n", sm->data_count, sm->max_data);
    if (strlen(sm->buffer) > 0) {
        printf("Buffer: %s\n", sm->buffer);
    }
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct