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
    if (sm == NULL || table == NULL || event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    enum State current = sm->current_state;
    if (current < 0 || current >= STATE_COUNT) {
        return 0;
    }
    
    enum State next_state = table->next_state[current][event];
    if (next_state < 0 || next_state >= STATE_COUNT) {
        return 0;
    }
    
    sm->current_state = next_state;
    return 1;
}

int simulate_data_processing(struct StateMachine *sm) {
    if (sm == NULL) {
        return 0;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("State: IDLE\n");
            break;
        case STATE_READING:
            if (sm->data_count < sm->max_data) {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = 'A' + (sm->data_count % 26);
                    sm->data_count++;
                }
            }
            printf("State: READING (data: %u/%u)\n", sm->data_count, sm->max_data);
            break;
        case STATE_PROCESSING:
            if (sm->buffer_pos > 0) {
                for (uint32_t i = 0; i < sm->buffer_pos; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
            }
            printf("State: PROCESSING\n");
            break;
        case STATE_WRITING:
            if (sm->buffer_pos > 0) {
                printf("Output: %.*s\n", (int)sm->buffer_pos, sm->buffer);
                sm->buffer_pos = 0;
            }
            printf("State: WRITING\n");
            break;
        case STATE_ERROR:
            printf("State: ERROR\n");
            break;
        default:
            return 0;
    }
    return 1;
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    init_transition_table(&table);
    init_state_machine(&sm, 10);
    
    enum Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS