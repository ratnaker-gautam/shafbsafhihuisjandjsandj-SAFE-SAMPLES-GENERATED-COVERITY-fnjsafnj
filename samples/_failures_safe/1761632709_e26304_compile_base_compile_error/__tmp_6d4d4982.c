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
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    size_t buffer_len;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

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
    
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_READING;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

void state_machine_init(struct StateMachine *sm, uint32_t max_data) {
    if (sm == NULL || max_data == 0 || max_data > 1000) {
        return;
    }
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL || event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return 0;
    }
    
    enum State next_state = tt->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) {
        return 0;
    }
    
    sm->current_state = next_state;
    return 1;
}

void simulate_reading(struct StateMachine *sm) {
    if (sm == NULL || sm->data_count >= sm->max_data) {
        return;
    }
    
    const char *sample_data = "Sample data chunk ";
    size_t data_len = strlen(sample_data);
    if (sm->buffer_len + data_len + 10 < sizeof(sm->buffer)) {
        int written = snprintf(sm->buffer + sm->buffer_len, 
                             sizeof(sm->buffer) - sm->buffer_len, 
                             "%s%d", sample_data, sm->data_count + 1);
        if (written > 0 && (size_t)written < sizeof(sm->buffer) - sm->buffer_len) {
            sm->buffer_len += written;
            sm->data_count++;
        }
    }
}

void simulate_processing(struct StateMachine *sm) {
    if (sm == NULL || sm->buffer_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < sm->buffer_len && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
}

void simulate_writing(struct StateMachine *sm) {
    if (sm == NULL || sm->buffer_len == 0) {
        return;
    }
    
    printf("Output: %s\n", sm->buffer);
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
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
    
    initialize_transition