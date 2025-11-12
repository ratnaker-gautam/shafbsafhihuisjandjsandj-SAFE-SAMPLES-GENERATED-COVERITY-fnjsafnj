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
    if (sm == NULL || table == NULL || event >= EVENT_COUNT) {
        return 0;
    }
    
    enum State next_state = table->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) {
        return 0;
    }
    
    sm->current_state = next_state;
    return 1;
}

int read_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) {
        return 0;
    }
    
    if (sm->buffer_pos >= sizeof(sm->buffer) - 1) {
        return 0;
    }
    
    sm->buffer[sm->buffer_pos] = 'A' + (sm->data_count % 26);
    sm->buffer_pos++;
    sm->buffer[sm->buffer_pos] = '\0';
    
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) {
        return 0;
    }
    
    if (sm->buffer_pos == 0) {
        return 0;
    }
    
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) {
        return 0;
    }
    
    if (sm->buffer_pos == 0) {
        return 0;
    }
    
    printf("Data %u: %s\n", sm->data_count + 1, sm->buffer);
    sm->data_count++;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    
    return 1;
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    init_transition_table(&table);
    init_state_machine(&sm, 5);
    
    printf("Starting state machine simulation\n");
    
    while (sm.data_count < sm.max_data) {
        switch (sm.current_state) {
            case STATE_IDLE:
                if (!handle_event(&sm, &table, EVENT_START)) {
                    printf("Failed to start\n");
                    return 1;
                }
                break;
                
            case STATE_READING:
                if (read_data(&sm)) {
                    if (!handle_event(&sm, &table, EVENT_DATA_READY)) {
                        printf