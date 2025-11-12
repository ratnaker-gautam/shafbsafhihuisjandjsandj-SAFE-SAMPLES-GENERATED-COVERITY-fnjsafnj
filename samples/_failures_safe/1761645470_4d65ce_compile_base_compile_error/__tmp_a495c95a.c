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
    EVENT_QUIT,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t error_count;
    uint32_t total_operations;
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
    sm->total_operations = 0;
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
    
    enum State current = sm->current_state;
    enum State next = tt->next_state[current][event];
    
    if (next == STATE_ERROR && event != EVENT_RESET) {
        sm->error_count++;
        return 0;
    }
    
    sm->current_state = next;
    
    if (event == EVENT_START || event == EVENT_RESET) {
        sm->data_value = 0;
        sm->processed_value = 0;
    }
    
    if (event == EVENT_DATA_READY) {
        sm->data_value = (sm->total_operations * 7 + 3) % 100;
    }
    
    if (event == EVENT_PROCESS_COMPLETE) {
        sm->processed_value = sm->data_value * 2;
    }
    
    if (event == EVENT_WRITE_COMPLETE) {
        sm->total_operations++;
    }
    
    return 1;
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE",
        "READING",
        "PROCESSING",
        "WRITING",
        "ERROR"
    };
    
    if (state < 0 || state >= STATE_COUNT) return "UNKNOWN";
    return names[state];
}

const char* event_to_string(enum Event event) {
    static const char* names[] = {
        "START",
        "DATA_READY",
        "PROCESS_COMPLETE",
        "WRITE_COMPLETE",
        "ERROR",
        "RESET",
        "QUIT"
    };
    
    if (event < 0 || event >= EVENT_COUNT) return "UNKNOWN";
    return names[event];
}

void print_state_machine(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current State: %s\n", state_to_string(sm->current_state));
    printf("Data Value: %u\n", sm->data_value);
    printf("Processed Value: %u\n", sm->processed_value);
    printf("Error Count: %u\n", sm->error_count);
    printf("Total Operations: %u\n", sm->total_operations);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable tt;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&tt);
    
    printf("State Machine Simulation\n");
    printf("Available events: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR,