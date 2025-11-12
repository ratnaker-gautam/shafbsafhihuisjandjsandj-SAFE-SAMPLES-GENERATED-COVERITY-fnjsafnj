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
    uint32_t operation_count;
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

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->operation_count = 0;
}

int state_machine_handle_event(struct StateMachine *sm, struct TransitionTable *table, enum Event event) {
    if (sm == NULL || table == NULL) return 0;
    if (event < 0 || event >= EVENT_COUNT) return 0;
    
    enum State next_state = table->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) return 0;
    
    sm->current_state = next_state;
    return 1;
}

void state_machine_read_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->current_state != STATE_READING) return;
    
    sm->data_value = (uint32_t)(rand() % 1000);
    sm->operation_count++;
}

void state_machine_process_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->current_state != STATE_PROCESSING) return;
    
    if (sm->data_value > 0 && sm->data_value < UINT32_MAX / 2) {
        sm->processed_value = sm->data_value * 2;
    } else {
        sm->error_count++;
    }
}

void state_machine_write_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->current_state != STATE_WRITING) return;
    
    printf("Processed value: %u\n", sm->processed_value);
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < 0 || state >= STATE_COUNT) return "UNKNOWN";
    return names[state];
}

const char* event_to_string(enum Event event) {
    static const char* names[] = {
        "START", "DATA_READY", "PROCESS_COMPLETE", 
        "WRITE_COMPLETE", "ERROR", "RESET"
    };
    if (event < 0 || event >= EVENT_COUNT) return "UNKNOWN";
    return names[event];
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    state_machine_init(&sm);
    initialize_transition_table(&table);
    
    srand(42);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET, 6=EXIT\n");
    
    int running = 1;
    while (running) {
        printf("\nCurrent state: %s\n", state_to_string(sm.current_state));
        printf("Operations: %u, Errors: %u\n", sm.operation_count, sm.error_count);
        printf("Enter command: ");