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
    uint32_t data_buffer[16];
    size_t data_size;
    uint32_t processed_data[16];
    size_t processed_size;
    uint32_t error_code;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

void initialize_transition_table(struct TransitionTable *tt) {
    if (tt == NULL) return;
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            tt->next_state[i][j] = STATE_ERROR;
        }
    }
    tt->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    tt->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    tt->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    tt->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

int handle_event(struct StateMachine *sm, const struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL) return 0;
    if (event >= EVENT_COUNT) return 0;
    enum State next_state = tt->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) return 0;
    sm->current_state = next_state;
    return 1;
}

int simulate_data_read(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_READING) return 0;
    sm->data_size = 8;
    for (size_t i = 0; i < sm->data_size; i++) {
        sm->data_buffer[i] = (uint32_t)(i * 10);
    }
    return 1;
}

int simulate_data_process(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_PROCESSING) return 0;
    if (sm->data_size == 0 || sm->data_size > 16) return 0;
    sm->processed_size = sm->data_size;
    for (size_t i = 0; i < sm->processed_size; i++) {
        sm->processed_data[i] = sm->data_buffer[i] + 5;
    }
    return 1;
}

int simulate_data_write(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_WRITING) return 0;
    if (sm->processed_size == 0 || sm->processed_size > 16) return 0;
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("Output[%zu]: %u\n", i, sm->processed_data[i]);
    }
    return 1;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WRITING: return "WRITING";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable tt;
    initialize_state_machine(&sm);
    initialize_transition_table(&tt);
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    if (!handle_event(&sm, &tt, EVENT_START)) {
        printf("Failed to handle EVENT_START\n");
        return 1;
    }
    printf("State after EVENT_START: %s\n", state_to_string(sm.current_state));
    if (!simulate_data_read(&sm)) {
        printf("Failed to read data\n");
        return 1;
    }
    if (!handle_event(&sm, &tt, EVENT_DATA_READY)) {
        printf("Failed to handle EVENT_DATA_READY\n");
        return 1;
    }
    printf("