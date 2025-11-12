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
    size_t data_count;
    uint32_t processed_data[16];
    size_t processed_count;
    uint32_t error_code;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
    int valid_transition[STATE_COUNT][EVENT_COUNT];
};

void initialize_transition_table(struct TransitionTable *table) {
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            table->valid_transition[i][j] = 0;
            table->next_state[i][j] = STATE_ERROR;
        }
    }

    table->valid_transition[STATE_IDLE][EVENT_START] = 1;
    table->next_state[STATE_IDLE][EVENT_START] = STATE_READING;

    table->valid_transition[STATE_READING][EVENT_DATA_READY] = 1;
    table->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    table->valid_transition[STATE_READING][EVENT_ERROR] = 1;
    table->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;

    table->valid_transition[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = 1;
    table->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    table->valid_transition[STATE_PROCESSING][EVENT_ERROR] = 1;
    table->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;

    table->valid_transition[STATE_WRITING][EVENT_WRITE_COMPLETE] = 1;
    table->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    table->valid_transition[STATE_WRITING][EVENT_ERROR] = 1;
    table->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;

    table->valid_transition[STATE_ERROR][EVENT_RESET] = 1;
    table->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) {
        return;
    }
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    for (size_t i = 0; i < 16; i++) {
        sm->data_buffer[i] = 0;
        sm->processed_data[i] = 0;
    }
}

int handle_event(struct StateMachine *sm, struct TransitionTable *table, enum Event event) {
    if (sm == NULL || table == NULL) {
        return 0;
    }
    if (event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return 0;
    }

    if (!table->valid_transition[sm->current_state][event]) {
        return 0;
    }

    sm->current_state = table->next_state[sm->current_state][event];
    return 1;
}

void simulate_data_read(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) {
        return;
    }
    if (sm->data_count >= 16) {
        return;
    }
    for (size_t i = 0; i < 4 && sm->data_count < 16; i++) {
        sm->data_buffer[sm->data_count] = (uint32_t)(sm->data_count + 1) * 10;
        sm->data_count++;
    }
}

void simulate_data_process(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) {
        return;
    }
    if (sm->processed_count >= 16) {
        return;
    }
    for (size_t i = 0; i < sm->data_count && sm->processed_count < 16; i++) {
        if (sm->data_buffer[i] > UINT32_MAX - 100) {
            sm->error_code = 1;
            return;
        }
        sm->processed_data[sm->processed_count] = sm->data_buffer[i] + 5;
        sm->processed_count++;
    }
}

void simulate_data_write(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) {
        return;
    }
    for (size_t i = 0; i < sm->processed_count; i++) {
        printf("Output: %u\n", sm->processed_data[i]);
    }
}

void reset_state_machine(struct StateMachine *