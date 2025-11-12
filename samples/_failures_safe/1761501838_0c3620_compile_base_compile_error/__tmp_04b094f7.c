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
    size_t buffer_size;
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

void initialize_state_machine(struct StateMachine *sm, uint32_t max_data) {
    if (sm == NULL || max_data == 0 || max_data > 1000) {
        return;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->buffer[0] = '\0';
    sm->buffer_size = 0;
}

int handle_event(struct StateMachine *sm, const struct TransitionTable *table, enum Event event) {
    if (sm == NULL || table == NULL || event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    enum State next_state = table->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) {
        return 0;
    }
    
    sm->current_state = next_state;
    return 1;
}

int simulate_reading(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) {
        return 0;
    }
    
    if (sm->data_count >= sm->max_data) {
        return 0;
    }
    
    const char *sample_data = "Sample data item ";
    size_t data_len = strlen(sample_data);
    
    if (data_len >= sizeof(sm->buffer)) {
        return 0;
    }
    
    strncpy(sm->buffer, sample_data, sizeof(sm->buffer) - 1);
    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
    
    char num_str[16];
    int written = snprintf(num_str, sizeof(num_str), "%u", sm->data_count + 1);
    if (written < 0 || (size_t)written >= sizeof(num_str)) {
        return 0;
    }
    
    if (strlen(sm->buffer) + strlen(num_str) < sizeof(sm->buffer)) {
        strcat(sm->buffer, num_str);
    }
    
    sm->buffer_size = strlen(sm->buffer);
    sm->data_count++;
    
    return 1;
}

int simulate_processing(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) {
        return 0;
    }
    
    if (sm->buffer_size == 0 || sm->buffer_size >= sizeof(sm->buffer)) {
        return 0;
    }
    
    for (size_t i = 0; i < sm->buffer_size; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return 1;
}

int simulate_writing(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) {
        return 0;
    }
    
    if (sm->buffer_size == 0) {
        return 0;
    }
    
    printf("Output: %s\n", sm->buffer);
    sm->buffer[0] = '\0';
    sm->buffer_size = 0;
    
    return 1;
}

const char* state