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
    uint32_t error_count;
    uint32_t total_operations;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

struct InputBuffer {
    char buffer[64];
    size_t position;
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->total_operations = 0;
}

void initialize_transition_table(struct TransitionTable *tt) {
    if (tt == NULL) return;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            tt->next_state[i][j] = STATE_ERROR;
        }
    }
    
    tt->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    tt->next_state[STATE_IDLE][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    tt->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_READING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    tt->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_PROCESSING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
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

int read_input(struct InputBuffer *ib) {
    if (ib == NULL) return 0;
    
    printf("Enter command (start, data, process, write, error, reset, quit): ");
    
    if (fgets(ib->buffer, sizeof(ib->buffer), stdin) == NULL) {
        return 0;
    }
    
    ib->position = 0;
    return 1;
}

enum Event parse_event(const struct InputBuffer *ib) {
    if (ib == NULL) return EVENT_COUNT;
    
    if (strncmp(ib->buffer, "start", 5) == 0) return EVENT_START;
    if (strncmp(ib->buffer, "data", 4) == 0) return EVENT_DATA_READY;
    if (strncmp(ib->buffer, "process", 7) == 0) return EVENT_PROCESS_COMPLETE;
    if (strncmp(ib->buffer, "write", 5) == 0) return EVENT_WRITE_COMPLETE;
    if (strncmp(ib->buffer, "error", 5) == 0) return EVENT_ERROR;
    if (strncmp(ib->buffer, "reset", 5) == 0) return EVENT_RESET;
    if (strncmp(ib->buffer, "quit", 4) == 0) return EVENT_COUNT;
    
    return EVENT_COUNT;
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

void process_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->data_value > UINT32_MAX - 100) {
        sm->processed_value = 0;
    } else {
        sm->processed_value = sm->data_value + 100;
    }
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable tt;
    struct InputBuffer ib;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&tt);
    memset(&ib,