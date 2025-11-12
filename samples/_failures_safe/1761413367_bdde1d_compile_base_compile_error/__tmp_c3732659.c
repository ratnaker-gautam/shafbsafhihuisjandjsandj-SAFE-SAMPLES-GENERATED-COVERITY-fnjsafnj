//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
    uint32_t write_count;
    uint32_t error_count;
    uint8_t is_running;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->write_count = 0;
    sm->error_count = 0;
    sm->is_running = 1;
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
    
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_READING;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

enum State handle_event(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL || event >= EVENT_COUNT) {
        return STATE_ERROR;
    }
    
    enum State current = sm->current_state;
    if (current >= STATE_COUNT) {
        return STATE_ERROR;
    }
    
    enum State next_state = tt->next_state[current][event];
    if (next_state >= STATE_COUNT) {
        return STATE_ERROR;
    }
    
    sm->current_state = next_state;
    return next_state;
}

void simulate_data_reading(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_READING) {
        sm->data_value = (sm->data_value + 1) % 1000;
    }
}

void simulate_data_processing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_PROCESSING) {
        if (sm->data_value > 0) {
            if (sm->data_value > UINT32_MAX / 2) {
                sm->current_state = STATE_ERROR;
                sm->error_count++;
            } else {
                sm->processed_value = sm->data_value * 2;
            }
        }
    }
}

void simulate_data_writing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_WRITING) {
        if (sm->write_count < UINT32_MAX - 1) {
            sm->write_count++;
        } else {
            sm->write_count = 0;
        }
    }
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable tt;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&tt);
    
    int cycle_count = 0;
    const int max_cycles = 50;
    
    while (sm.is_running && cycle_count < max_cycles) {
        enum Event current_event;
        
        switch (sm.current_state) {
            case STATE_IDLE:
                current_event = EVENT_START;
                break;
            case STATE_READING:
                simulate_data_reading(&sm);
                current_event = EVENT_DATA_READY;
                break;
            case STATE_PROCESSING:
                simulate_data_processing(&sm);
                current_event = EVENT_PROCESS_COMPLETE;
                break;
            case STATE_WRITING:
                simulate_data_writing(&sm);
                current_event = EVENT_WRITE_COMPLETE;
                break;
            case STATE_ERROR:
                current_event = EVENT_RESET;
                break;
            default:
                current_event = EVENT_RESET;
                break;
        }
        
        handle_event(&sm, &tt, current_event);
        
        printf("Cycle %d: State=%d,