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
    EVENT_QUIT,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t write_count;
    uint32_t error_count;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

void initialize_transition_table(struct TransitionTable *table) {
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            table->next_state[i][j] = STATE_ERROR;
        }
    }
    
    table->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    table->next_state[STATE_IDLE][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    table->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_READING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    table->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_PROCESSING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    table->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

void initialize_state_machine(struct StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->write_count = 0;
    sm->error_count = 0;
}

int handle_event(struct StateMachine *sm, struct TransitionTable *table, enum Event event) {
    if (sm == NULL || table == NULL) {
        return 0;
    }
    
    if (event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    enum State current = sm->current_state;
    if (current < 0 || current >= STATE_COUNT) {
        return 0;
    }
    
    enum State next = table->next_state[current][event];
    if (next < 0 || next >= STATE_COUNT) {
        return 0;
    }
    
    if (next == STATE_ERROR && current != STATE_ERROR) {
        sm->error_count++;
        return 0;
    }
    
    sm->current_state = next;
    return 1;
}

int simulate_data_read(struct StateMachine *sm) {
    if (sm == NULL) {
        return 0;
    }
    
    if (sm->current_state != STATE_READING) {
        return 0;
    }
    
    sm->data_value = (sm->data_value + 1) % 1000;
    return 1;
}

int simulate_data_process(struct StateMachine *sm) {
    if (sm == NULL) {
        return 0;
    }
    
    if (sm->current_state != STATE_PROCESSING) {
        return 0;
    }
    
    if (sm->data_value < 1000) {
        if (sm->data_value > UINT32_MAX / 2) {
            return 0;
        }
        sm->processed_value = sm->data_value * 2;
        return 1;
    }
    
    return 0;
}

int simulate_data_write(struct StateMachine *sm) {
    if (sm == NULL) {
        return 0;
    }
    
    if (sm->current_state != STATE_WRITING) {
        return 0;
    }
    
    if (sm->write_count < UINT32_MAX) {
        sm->write_count++;
        return 1;
    }
    
    return 0;
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE",
        "READING",
        "PROCESSING",
        "WRITING",
        "ERROR"
    };
    
    if (state >= 0 && state < STATE_COUNT) {
        return names[state];
    }
    return "UNKNOWN";
}

const char* event_to_string(enum Event event) {
    static const char* names[] = {
        "START",
        "DATA_READY",
        "PROCESS_COMPLETE",
        "WRITE_COMPLETE",
        "ERROR",
        "RESET",
        "QUIT"
    };
    
    if (event >= 0 && event < EVENT_COUNT) {
        return names[event];
    }
    return "UNKNOWN";
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;