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
    uint32_t buffer_pos;
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

void initialize_state_machine(struct StateMachine *sm, uint32_t max_data) {
    if (sm == NULL || max_data == 0 || max_data > 1000) {
        return;
    }
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_event(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL || event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    enum State next_state = tt->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) {
        return 0;
    }
    
    sm->current_state = next_state;
    return 1;
}

void simulate_data_processing(struct StateMachine *sm) {
    if (sm == NULL || sm->buffer_pos >= sizeof(sm->buffer)) {
        return;
    }
    
    for (uint32_t i = 0; i < sm->buffer_pos && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
}

int read_input_data(struct StateMachine *sm) {
    if (sm == NULL || sm->buffer_pos >= sizeof(sm->buffer)) {
        return 0;
    }
    
    printf("Enter data (max %zu chars): ", sizeof(sm->buffer) - sm->buffer_pos - 1);
    if (fgets(sm->buffer + sm->buffer_pos, sizeof(sm->buffer) - sm->buffer_pos, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->buffer + sm->buffer_pos);
    if (len > 0 && sm->buffer[sm->buffer_pos + len - 1] == '\n') {
        sm->buffer[sm->buffer_pos + len - 1] = '\0';
        len--;
    }
    
    sm->buffer_pos += len;
    return len > 0;
}

void write_output_data(struct StateMachine *sm) {
    if (sm == NULL || sm->buffer_pos == 0) {
        return;
    }
    
    printf("Processed data: %s\n", sm->buffer);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->data_count++;
}

const char* state_to_string(enum State state) {
    switch(state) {
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
    
    initialize_transition_table(&