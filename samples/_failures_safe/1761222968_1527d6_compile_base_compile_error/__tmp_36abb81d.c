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
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t write_count;
    uint32_t error_count;
    uint32_t max_cycles;
};

struct Transition {
    enum State next_state;
    int (*action)(struct StateMachine*);
};

struct StateTable {
    struct Transition transitions[EVENT_COUNT];
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->write_count = 0;
    sm->error_count = 0;
    return 0;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_value > 1000) return -1;
    sm->data_value += 42;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_value > UINT32_MAX / 2) return -1;
    sm->processed_value = sm->data_value * 2;
    return 0;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->write_count >= sm->max_cycles) return -1;
    sm->write_count++;
    return 0;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->error_count >= 5) return -1;
    sm->error_count++;
    return 0;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_value = 0;
    sm->processed_value = 0;
    return 0;
}

void initialize_state_table(struct StateTable table[STATE_COUNT]) {
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            table[i].transitions[j].next_state = STATE_ERROR;
            table[i].transitions[j].action = action_to_error;
        }
    }

    table[STATE_IDLE].transitions[EVENT_START].next_state = STATE_READING;
    table[STATE_IDLE].transitions[EVENT_START].action = action_idle_to_reading;

    table[STATE_READING].transitions[EVENT_DATA_READY].next_state = STATE_PROCESSING;
    table[STATE_READING].transitions[EVENT_DATA_READY].action = action_reading_to_processing;

    table[STATE_PROCESSING].transitions[EVENT_PROCESS_COMPLETE].next_state = STATE_WRITING;
    table[STATE_PROCESSING].transitions[EVENT_PROCESS_COMPLETE].action = action_processing_to_writing;

    table[STATE_WRITING].transitions[EVENT_WRITE_COMPLETE].next_state = STATE_IDLE;
    table[STATE_WRITING].transitions[EVENT_WRITE_COMPLETE].action = action_writing_to_idle;

    table[STATE_ERROR].transitions[EVENT_RESET].next_state = STATE_IDLE;
    table[STATE_ERROR].transitions[EVENT_RESET].action = action_error_to_idle;
}

int process_event(struct StateMachine* sm, struct StateTable table[STATE_COUNT], enum Event event) {
    if (sm == NULL || table == NULL) return -1;
    if (sm->current_state >= STATE_COUNT || event >= EVENT_COUNT) return -1;

    struct Transition* transition = &table[sm->current_state].transitions[event];
    if (transition->action == NULL) return -1;

    int result = transition->action(sm);
    if (result != 0) return -1;

    sm->current_state = transition->next_state;
    return 0;
}

void print_state_info(const struct StateMachine* sm) {
    if (sm == NULL) return;
    const char* state_names[] = {"IDLE", "READING", "PROCESSING", "WRITING", "ERROR"};
    printf("State: %s, Data: %u, Processed: %u, Writes: %u, Errors: %u\n",
           state_names[sm->current_state], sm->data_value, sm->processed_value,
           sm->write_count, sm->error_count);
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.processed_value = 0;
    sm.write_count = 0;
    sm.error_count = 0;
    sm.max_cycles = 5;

    struct StateTable table[STATE_COUNT];
    initialize_state_table(table);

    enum Event events[] = {
        EVENT