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

int read_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return 0;
    
    if (sm->data_count >= 16) {
        sm->error_code = 1;
        return 0;
    }
    
    for (size_t i = 0; i < 4; i++) {
        if (sm->data_count < 16) {
            sm->data_buffer[sm->data_count] = (uint32_t)(sm->data_count + 1) * 10;
            sm->data_count++;
        }
    }
    
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return 0;
    
    if (sm->processed_count >= 16) {
        sm->error_code = 2;
        return 0;
    }
    
    for (size_t i = 0; i < sm->data_count; i++) {
        if (sm->processed_count < 16) {
            sm->processed_data[sm->processed_count] = sm->data_buffer[i] * 2;
            sm->processed_count++;
        }
    }
    
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    
    printf("Processed data: ");
    for (size_t i = 0; i < sm->processed_count; i++) {
        printf("%u ", sm->processed_data[i]);
    }
    printf("\n");
    
    return 1;
}

void handle_event(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL) return;
    if (event >= EVENT_COUNT) return;
    
    enum State next_state = tt->next_state[sm->current_state][event];
    if (next_state >= STATE_COUNT) return;
    
    sm->current_state = next_state;
    
    switch (sm->current_state) {
        case STATE_READING:
            if (!read_data(sm)) {
                sm->current_state = STATE_ERROR;
                sm->error_code = 3;
            }
            break;
        case STATE_PROCESSING:
            if (!process_data(sm)) {
                sm->current_state = STATE_ERROR;
                sm->error_code = 4;
            }
            break;
        case STATE_WRITING:
            if (!write_data(sm)) {
                sm->current_state = STATE_ERROR;
                sm->error_code = 5;
            }
            break;
        case STATE_ERROR:
            printf("Error occurred: code %u\n", sm->error_code);
            break