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
    char payload[128];
    uint32_t payload_length;
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
    if (event_data->payload_length >= sizeof(event_data->payload)) return 0;
    return 1;
}

int process_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READY;
        return 1;
    }
    return 0;
}

int process_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_DATA_READY) {
        if (event_data->payload_length > 0 && event_data->payload_length < sizeof(sm->buffer)) {
            strncpy(sm->buffer, event_data->payload, event_data->payload_length);
            sm->buffer[event_data->payload_length] = '\0';
            sm->current_state = STATE_PROCESSING;
            return 1;
        }
    } else if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int process_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_SUCCESS) {
        sm->data_count++;
        if (sm->data_count < sm->max_data) {
            sm->current_state = STATE_READY;
        } else {
            sm->current_state = STATE_COMPLETE;
        }
        return 1;
    } else if (event_data->event_type == EVENT_FAILURE) {
        sm->current_state = STATE_ERROR;
        return 1;
    } else if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int process_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        return 1;
    }
    return 0;
}

int process_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return 1;
    }
    return 0;
}

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    switch (sm->current_state) {
        case STATE_IDLE:
            return process_idle_state(sm, event_data);
        case STATE_READY:
            return process_ready_state(sm, event_data);
        case STATE_PROCESSING:
            return process_processing_state(sm, event_data);
        case STATE_COMPLETE:
            return process_complete_state(sm, event_data);
        case STATE_ERROR:
            return process_error_state(sm, event_data);
        default:
            return 0;
    }
}

void print_state_machine_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u/%u\n", sm->data_count, sm->max_data);
    if (strlen(sm->buffer) > 0) {
        printf("Buffer: %s\n", sm->buffer);
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    struct EventData event;
    int running = 1;
    char input[256];