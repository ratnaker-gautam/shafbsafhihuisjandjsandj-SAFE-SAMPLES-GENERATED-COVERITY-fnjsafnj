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
                if (event_data->data_len > 0 && event_data->data_len < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event_data->data, event_data->data_len);
                    sm->buffer[event_data->data_len] = '\0';
                    new_state = STATE_PROCESSING;
                } else {
                    new_state = STATE_ERROR;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_SUCCESS) {
                if (sm->data_count < sm->max_data - 1) {
                    sm->data_count++;
                    new_state = STATE_COMPLETE;
                } else {
                    new_state = STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_FAILURE) {
                new_state = STATE_ERROR;
            }
            break;
        case STATE_COMPLETE:
            if (event_data->event_type == EVENT_RESET) {
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                new_state = STATE_IDLE;
            }
            break;
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                new_state = STATE_IDLE;
            }
            break;
        default:
            new_state = STATE_ERROR;
            break;
    }

    sm->current_state = new_state;
    return 1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current State: %s, Data Count: %u\n", 
           state_names[sm->current_state], sm->data_count);
    if (sm->current_state == STATE_PROCESSING || sm->current_state == STATE_COMPLETE) {
        printf("Buffer: %s\n", sm->buffer);
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);

    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample Data", 11},
        {EVENT_SUCCESS, "", 0},
        {EVENT_RESET, "", 0},
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Another Sample", 14},
        {EVENT_FAILURE, "", 0},
        {EVENT_RESET, "", 0}
    };

    size_t num_events = sizeof(events) / sizeof(events[0]);

    for (size_t i = 0; i < num_events; i++) {
        printf("Processing Event %zu: ", i + 1);
        switch (events[i].event_type) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA_READY: printf("DATA_READY"); break;
            case EVENT_PROCESS: printf("PROCESS"); break;
            case EVENT_SUCCESS: printf("SUCCESS"); break;
            case EVENT_FAILURE: printf("FAILURE"); break;
            case EVENT_RESET: printf("RESET"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");

        if (!process_state_transition(&sm, &events[i])) {
            printf("Invalid event or state machine\n");
            return 1;
        }

        print_state_info(&sm);
        printf("---\n");
    }

    return 0;
}