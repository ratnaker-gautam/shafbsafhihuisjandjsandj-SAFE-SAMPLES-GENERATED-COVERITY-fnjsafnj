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
    
    table->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_READING;
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

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) {
        return;
    }
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    enum State current = sm->current_state;
    if (current >= 0 && current < STATE_COUNT) {
        printf("Current state: %s, Data processed: %u/%u\n", 
               state_names[current], sm->data_count, sm->max_data);
    }
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    initialize_transition_table(&table);
    initialize_state_machine(&sm, 5);
    
    printf("State Machine Simulation Started\n");
    
    while (sm.data_count < sm.max_data && sm.current_state != STATE_ERROR) {
        print_state_info(&sm);
        
        switch (sm.current_state) {
            case STATE_IDLE:
                printf("Enter 'start' to begin: ");
                if (fgets(sm.buffer, sizeof(sm.buffer), stdin) != NULL) {
                    sm.buffer[strcspn(sm.buffer, "\n")] = 0;
                    if (strcmp(sm.buffer, "start") == 0) {
                        handle_event(&sm, &table, EVENT_START);
                    }
                }
                break;
                
            case STATE_READING:
                printf("Enter data to process: ");
                if (fgets(sm.buffer, sizeof(sm.buffer), stdin) != NULL) {
                    sm.buffer_pos = strcspn(s