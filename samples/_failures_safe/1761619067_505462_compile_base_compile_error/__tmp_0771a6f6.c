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
    uint32_t data_buffer[16];
    size_t data_count;
    uint32_t processed_value;
    int error_code;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_value = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
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
    if (sm == NULL || tt == NULL) return -1;
    if (event < 0 || event >= EVENT_COUNT) return -1;
    
    enum State next_state = tt->next_state[sm->current_state][event];
    if (next_state == STATE_ERROR) return -1;
    
    sm->current_state = next_state;
    return 0;
}

int read_data(struct StateMachine *sm) {
    if (sm == NULL) return -1;
    if (sm->current_state != STATE_READING) return -1;
    
    printf("Enter up to 16 numbers (0-1000), one per line. Enter -1 to finish:\n");
    sm->data_count = 0;
    
    for (int i = 0; i < 16; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            sm->error_code = 1;
            return -1;
        }
        
        if (value == -1) break;
        
        if (value < 0 || value > 1000) {
            sm->error_code = 2;
            return -1;
        }
        
        sm->data_buffer[i] = (uint32_t)value;
        sm->data_count++;
    }
    
    if (sm->data_count == 0) {
        sm->error_code = 3;
        return -1;
    }
    
    return 0;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL) return -1;
    if (sm->current_state != STATE_PROCESSING) return -1;
    if (sm->data_count == 0) return -1;
    
    uint32_t sum = 0;
    for (size_t i = 0; i < sm->data_count; i++) {
        if (sum > UINT32_MAX - sm->data_buffer[i]) {
            sm->error_code = 4;
            return -1;
        }
        sum += sm->data_buffer[i];
    }
    
    sm->processed_value = sum / sm->data_count;
    return 0;
}

int write_result(struct StateMachine *sm) {
    if (sm == NULL) return -1;
    if (sm->current_state != STATE_WRITING) return -1;
    
    printf("Processed %zu numbers. Average: %u\n", sm->data_count, sm->processed_value);
    return 0;
}

void print_state(enum State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (state >= 0 && state < STATE_COUNT) {
        printf("Current state: %s\n",