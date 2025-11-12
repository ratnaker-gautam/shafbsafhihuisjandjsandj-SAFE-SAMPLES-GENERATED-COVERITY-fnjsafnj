//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

int validate_event_data(const struct EventData *event) {
    if (event == NULL) return 0;
    if (event->event_type < EVENT_START || event->event_type > EVENT_RESET) return 0;
    if (event->data_len >= sizeof(event->data)) return 0;
    return 1;
}

int process_state_transition(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || !validate_event_data(event)) return 0;

    enum State next_state = sm->current_state;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                next_state = STATE_READING;
            }
            break;
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data) {
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event->event_type == EVENT_ERROR) {
                next_state = STATE_ERROR;
            }
            break;
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                next_state = STATE_WRITING;
            } else if (event->event_type == EVENT_ERROR) {
                next_state = STATE_ERROR;
            }
            break;
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                sm->data_count++;
                if (sm->data_count >= sm->max_data) {
                    next_state = STATE_DONE;
                } else {
                    next_state = STATE_READING;
                }
            } else if (event->event_type == EVENT_ERROR) {
                next_state = STATE_ERROR;
            }
            break;
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                next_state = STATE_IDLE;
            }
            break;
        case STATE_DONE:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                next_state = STATE_IDLE;
            }
            break;
        default:
            next_state = STATE_ERROR;
            break;
    }

    sm->current_state = next_state;
    return 1;
}

int handle_event_data(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || !validate_event_data(event)) return 0;

    if (event->event_type == EVENT_DATA_READY) {
        if (event->data_len > 0 && sm->buffer_pos + event->data_len < sizeof(sm->buffer)) {
            memcpy(sm->buffer + sm->buffer_pos, event->data, event->data_len);
            sm->buffer_pos += event->data_len;
        }
    }

    return process_state_transition(sm, event);
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    printf("State: %s, Data Count: %u/%u, Buffer Used: %u\n",
           state_names[sm->current_state],
           sm->data_count,
           sm->max_data,
           sm->buffer_pos);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);

    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_DATA_READY, "Sample data 2", 13},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_RESET, "", 0}
    };

    size_t num_events = sizeof(events) / sizeof(events[0]);

    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event %zu: ", i);
        switch (events[i].event_type) {
            case EVENT_START: printf("START"); break;
            case EVENT