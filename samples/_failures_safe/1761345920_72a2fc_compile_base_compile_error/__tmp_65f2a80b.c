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

int process_state_transition(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;

    enum State new_state = sm->current_state;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                new_state = STATE_READY;
            }
            break;
        case STATE_READY:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data) {
                    size_t copy_len = event_data->data_len;
                    if (copy_len > sizeof(sm->buffer) - 1) copy_len = sizeof(sm->buffer) - 1;
                    memcpy(sm->buffer, event_data->data, copy_len);
                    sm->buffer[copy_len] = '\0';
                    sm->data_count++;
                    new_state = STATE_PROCESSING;
                } else {
                    new_state = STATE_ERROR;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_SUCCESS) {
                new_state = STATE_COMPLETE;
            } else if (event_data->event_type == EVENT_FAILURE) {
                new_state = STATE_ERROR;
            }
            break;
        case STATE_COMPLETE:
            if (event_data->event_type == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        default:
            return 0;
    }

    if (new_state != sm->current_state) {
        sm->current_state = new_state;
        return 1;
    }
    return 0;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS: return "PROCESS";
        case EVENT_SUCCESS: return "SUCCESS";
        case EVENT_FAILURE: return "FAILURE";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);

    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_SUCCESS, "", 0},
        {EVENT_RESET, "", 0},
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Another sample", 14},
        {EVENT_FAILURE, "", 0},
        {EVENT_RESET, "", 0}
    };

    size_t num_events = sizeof(events) / sizeof(events[0]);

    printf("Initial state: %s\n", state_to_string(sm.current_state));

    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event: %s", event_to_string(events[i].event_type));
        if (events[i].data_len > 0) {
            printf(" with data: %.*s", (int)events[i].data_len, events[i].data);
        }
        printf("\n");

        if (process_state_transition(&sm, &events[i])) {
            printf("State changed to: %s\n", state_to_string