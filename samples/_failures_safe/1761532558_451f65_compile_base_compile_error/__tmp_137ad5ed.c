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
    
    if (sm->data_count >= sm->max_data) {
        return 0;
    }
    
    printf("Enter data (max 255 chars): ");
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
    }
    
    sm->buffer_pos = 0;
    sm->data_count++;
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(sm->buffer); i++) {
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
    
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

void print_state(enum State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    initialize_transition_table(&table);
    initialize_state_machine(&sm, 5);
    
    printf("State Machine Demo - Processing up to 5 data items\n");
    printf("Commands: start, data, process, write, error, reset, quit\n");
    
    char input[64];
    int running = 1;
    
    while (running && sm.data_count < sm.max