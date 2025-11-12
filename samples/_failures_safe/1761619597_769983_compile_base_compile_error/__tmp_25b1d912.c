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

int process_state_transition(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;

    enum State next_state = sm->current_state;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                next_state = STATE_READING;
            }
            break;
        case STATE_READING:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data) {
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                next_state = STATE_ERROR;
            }
            break;
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
                next_state = STATE_WRITING;
            } else if (event_data->event_type == EVENT_ERROR) {
                next_state = STATE_ERROR;
            }
            break;
        case STATE_WRITING:
            if (event_data->event_type == EVENT_WRITE_COMPLETE) {
                sm->data_count++;
                if (sm->data_count >= sm->max_data) {
                    next_state = STATE_DONE;
                } else {
                    next_state = STATE_READING;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                next_state = STATE_ERROR;
            }
            break;
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
        case STATE_DONE:
            if (event_data->event_type == EVENT_RESET) {
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

int handle_event_data(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;

    switch (event_data->event_type) {
        case EVENT_DATA_READY:
            if (sm->buffer_pos + event_data->data_len < sizeof(sm->buffer)) {
                memcpy(sm->buffer + sm->buffer_pos, event_data->data, event_data->data_len);
                sm->buffer_pos += event_data->data_len;
            }
            break;
        case EVENT_PROCESS_COMPLETE:
            sm->processed_count++;
            break;
        case EVENT_RESET:
            state_machine_init(sm);
            break;
        default:
            break;
    }

    return process_state_transition(sm, event_data);
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u/%u\n", sm->data_count, sm->max_data);
    printf("Processed Count: %u\n", sm->processed_count);
    printf("Buffer Position: %u\n", sm->buffer_pos);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);

    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample1", 7},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0},
        {EVENT_DATA_READY, "Data2", 5},
        {EVENT_PROCESS_COMPLETE, "", 0},
        {EVENT_WRITE_COMPLETE, "", 0