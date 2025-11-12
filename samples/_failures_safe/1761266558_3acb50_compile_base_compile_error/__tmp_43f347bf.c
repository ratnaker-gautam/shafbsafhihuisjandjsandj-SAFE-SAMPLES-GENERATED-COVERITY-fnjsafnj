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

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
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

int state_machine_validate(const struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state < STATE_IDLE || sm->current_state > STATE_DONE) return 0;
    if (sm->max_data == 0 || sm->max_data > 1000) return 0;
    if (sm->data_count > sm->max_data) return 0;
    if (sm->buffer_pos >= sizeof(sm->buffer)) return 0;
    return 1;
}

enum State state_machine_transition(struct StateMachine *sm, enum Event event) {
    if (!state_machine_validate(sm)) return STATE_ERROR;

    static const struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READING, EVENT_RESET, STATE_IDLE},
        {STATE_PROCESSING, EVENT_RESET, STATE_IDLE},
        {STATE_WRITING, EVENT_RESET, STATE_IDLE}
    };
    const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return sm->current_state;
        }
    }
    return sm->current_state;
}

int simulate_data_read(struct StateMachine *sm) {
    if (!state_machine_validate(sm)) return 0;
    if (sm->current_state != STATE_READING) return 0;
    if (sm->data_count >= sm->max_data) return 0;

    char data[] = "SampleData";
    size_t data_len = strlen(data);
    if (sm->buffer_pos + data_len >= sizeof(sm->buffer)) return 0;

    memcpy(sm->buffer + sm->buffer_pos, data, data_len);
    sm->buffer_pos += data_len;
    sm->data_count++;
    return 1;
}

int simulate_data_process(struct StateMachine *sm) {
    if (!state_machine_validate(sm)) return 0;
    if (sm->current_state != STATE_PROCESSING) return 0;
    if (sm->buffer_pos == 0) return 0;

    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        sm->buffer[i] = toupper(sm->buffer[i]);
    }
    sm->processed_count++;
    return 1;
}

int simulate_data_write(struct StateMachine *sm) {
    if (!state_machine_validate(sm)) return 0;
    if (sm->current_state != STATE_WRITING) return 0;
    if (sm->buffer_pos == 0) return 0;

    printf("Processed data: %.*s\n", (int)sm->buffer_pos, sm->buffer);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

void print_state_info(const struct StateMachine *sm) {
    if (!state_machine_validate(sm)) return;
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    printf("Current state: %s, Data count: %u/%u, Processed: %u\n",
           state_names[sm->current_state], sm->data_count, sm->max_data, sm->processed_count);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);

    printf("State Machine Simulation Started\n");
    print_state_info(&sm);

    while (sm.current_state != STATE_DONE && sm.current_state !=