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
    uint8_t error_code;
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
    sm->error_code = 0;
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
    
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

int validate_event(enum Event event) {
    return event >= 0 && event < EVENT_COUNT;
}

int validate_state(enum State state) {
    return state >= 0 && state < STATE_COUNT;
}

void handle_state_idle(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || !validate_event(event)) return;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing pipeline\n");
            break;
        case EVENT_RESET:
            printf("System reset in idle state\n");
            break;
        default:
            break;
    }
}

void handle_state_reading(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || !validate_event(event)) return;
    
    switch (event) {
        case EVENT_DATA_READY:
            sm->data_value = 42;
            printf("Data read: %u\n", sm->data_value);
            break;
        case EVENT_ERROR:
            sm->error_code = 1;
            printf("Error during data reading\n");
            break;
        case EVENT_RESET:
            printf("Reset during reading\n");
            break;
        default:
            break;
    }
}

void handle_state_processing(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || !validate_event(event)) return;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_value > 0) {
                sm->processed_value = sm->data_value * 2;
                printf("Data processed: %u -> %u\n", sm->data_value, sm->processed_value);
            }
            break;
        case EVENT_ERROR:
            sm->error_code = 2;
            printf("Error during data processing\n");
            break;
        case EVENT_RESET:
            printf("Reset during processing\n");
            break;
        default:
            break;
    }
}

void handle_state_writing(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || !validate_event(event)) return;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data written: %u\n", sm->processed_value);
            break;
        case EVENT_ERROR:
            sm->error_code = 3;
            printf("Error during data writing\n");
            break;
        case EVENT_RESET:
            printf("Reset during writing\n");
            break;
        default:
            break;
    }
}

void handle_state_error(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || !validate_event(event)) return;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void process_event(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt ==