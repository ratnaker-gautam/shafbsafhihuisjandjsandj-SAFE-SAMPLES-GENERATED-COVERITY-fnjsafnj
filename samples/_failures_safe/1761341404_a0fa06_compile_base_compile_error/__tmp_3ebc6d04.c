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

void init_transition_table(struct TransitionTable *table) {
    if (table == NULL) return;
    
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
    
    table->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_READING;
    table->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

void init_state_machine(struct StateMachine *sm, uint32_t max_data) {
    if (sm == NULL || max_data == 0 || max_data > 1000) {
        return;
    }
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_event(struct StateMachine *sm, const struct TransitionTable *table, enum Event event) {
    if (sm == NULL || table == NULL || event < 0 || event >= EVENT_COUNT) {
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
        return 0;
    }
    
    sm->current_state = next;
    return 1;
}

int simulate_data_reading(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) {
        return 0;
    }
    
    if (sm->data_count >= sm->max_data) {
        return 0;
    }
    
    const char *sample_data = "Sample data chunk ";
    size_t data_len = strlen(sample_data);
    
    if (sm->buffer_pos + data_len + 10 >= sizeof(sm->buffer)) {
        return 0;
    }
    
    int written = snprintf(sm->buffer + sm->buffer_pos, 
                          sizeof(sm->buffer) - sm->buffer_pos,
                          "%s%d\n", sample_data, sm->data_count + 1);
    
    if (written > 0 && (size_t)written < sizeof(sm->buffer) - sm->buffer_pos) {
        sm->buffer_pos += written;
        sm->data_count++;
        return 1;
    }
    
    return 0;
}

int simulate_data_processing(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) {
        return 0;
    }
    
    for (size_t i = 0; i < sm->buffer_pos && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    return 1;
}

int simulate_data_writing(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) {
        return 0;
    }
    
    if (sm->buffer_pos > 0) {
        size_t to_write = sm->buffer_pos;
        if (to_write > sizeof(sm->buffer)) {
            to_write = sizeof(sm->buffer);
        }
        
        size_t written = fwrite(sm->buffer, 1, to_write