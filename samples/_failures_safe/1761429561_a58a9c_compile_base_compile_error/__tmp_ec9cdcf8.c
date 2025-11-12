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

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->operation_count = 0;
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
    if (event < 0 || event >= EVENT_COUNT) return 0;
    
    enum State next_state = tt->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) return 0;
    
    sm->current_state = next_state;
    return 1;
}

int simulate_data_reading(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_READING) return 0;
    
    sm->data_value = (sm->operation_count * 7 + 3) % 100;
    return 1;
}

int simulate_data_processing(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_PROCESSING) return 0;
    
    if (sm->data_value > 90) {
        return 0;
    }
    
    sm->processed_value = sm->data_value * 2;
    return 1;
}

int simulate_data_writing(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_WRITING) return 0;
    
    printf("Operation %u: Processed value = %u\n", sm->operation_count, sm->processed_value);
    sm->operation_count++;
    return 1;
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
        "START", "DATA_READY", "PROCESS_COMPLETE", "WRITE_COMPLETE", "ERROR", "RESET"
    };
    if (event < 0 || event >= EVENT_COUNT) return "UNKNOWN";
    return names[event];
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable tt;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&tt);
    
    printf("State Machine Simulation Started\n");
    
    for (int cycle = 0; cycle < 10; cycle++) {
        printf("\nCycle %d - Current State: %s\n", cycle, state_to_string(sm.current_state));
        
        enum Event event;
        
        switch (sm.current_state) {
            case STATE_IDLE:
                event = EVENT_START;
                break;
            case STATE_READING:
                if (simulate_data_reading(&sm)) {
                    event = EVENT_DATA_READ