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
    uint32_t processed_data[16];
    size_t processed_count;
    uint32_t error_code;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
    int valid_transition[STATE_COUNT][EVENT_COUNT];
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

void initialize_transition_table(struct TransitionTable *tt) {
    if (tt == NULL) return;
    memset(tt->valid_transition, 0, sizeof(tt->valid_transition));
    memset(tt->next_state, 0, sizeof(tt->next_state));
    
    tt->valid_transition[STATE_IDLE][EVENT_START] = 1;
    tt->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    
    tt->valid_transition[STATE_READING][EVENT_DATA_READY] = 1;
    tt->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    tt->valid_transition[STATE_READING][EVENT_ERROR] = 1;
    tt->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    
    tt->valid_transition[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = 1;
    tt->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    tt->valid_transition[STATE_PROCESSING][EVENT_ERROR] = 1;
    tt->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    
    tt->valid_transition[STATE_WRITING][EVENT_WRITE_COMPLETE] = 1;
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    tt->valid_transition[STATE_WRITING][EVENT_ERROR] = 1;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    
    tt->valid_transition[STATE_ERROR][EVENT_RESET] = 1;
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

int handle_event(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL) return 0;
    if (event >= EVENT_COUNT || sm->current_state >= STATE_COUNT) return 0;
    
    if (!tt->valid_transition[sm->current_state][event]) {
        return 0;
    }
    
    enum State old_state = sm->current_state;
    sm->current_state = tt->next_state[old_state][event];
    
    switch (event) {
        case EVENT_START:
            sm->data_count = 0;
            sm->processed_count = 0;
            sm->error_code = 0;
            break;
        case EVENT_DATA_READY:
            if (sm->data_count < 16) {
                sm->data_buffer[sm->data_count] = (uint32_t)(sm->data_count + 1) * 10;
                sm->data_count++;
            }
            break;
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_count > 0 && sm->processed_count < 16) {
                size_t process_limit = sm->data_count;
                if (process_limit > 16 - sm->processed_count) {
                    process_limit = 16 - sm->processed_count;
                }
                for (size_t i = 0; i < process_limit; i++) {
                    sm->processed_data[sm->processed_count] = sm->data_buffer[i] * 2;
                    sm->processed_count++;
                }
                sm->data_count = 0;
            }
            break;
        case EVENT_WRITE_COMPLETE:
            sm->processed_count = 0;
            break;
        case EVENT_ERROR:
            sm->error_code = 1;
            break;
        case EVENT_RESET:
            sm->error_code = 0;
            sm->data_count = 0;
            sm->processed_count = 0;
            break;
        default:
            break;
    }
    
    return 1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    printf("Current State: %s\n", state