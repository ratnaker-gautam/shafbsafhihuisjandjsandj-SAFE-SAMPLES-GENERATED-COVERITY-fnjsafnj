//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(struct StateMachine*);

struct StateMachine {
    State current_state;
    StateHandler handlers[STATE_COUNT][EVENT_COUNT];
    int data_value;
    int error_code;
};

void handle_idle_start(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Transition: IDLE -> READY\n");
    sm->current_state = STATE_READY;
}

void handle_idle_other(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error: Invalid event for IDLE state\n");
}

void handle_ready_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Transition: READY -> PROCESSING\n");
    sm->current_state = STATE_PROCESSING;
}

void handle_ready_other(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error: Invalid event for READY state\n");
}

void handle_processing_process(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Processing data...\n");
}

void handle_processing_success(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Transition: PROCESSING -> COMPLETE\n");
    sm->current_state = STATE_COMPLETE;
}

void handle_processing_failure(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Transition: PROCESSING -> ERROR\n");
    sm->current_state = STATE_ERROR;
}

void handle_processing_other(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error: Invalid event for PROCESSING state\n");
}

void handle_complete_reset(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Transition: COMPLETE -> IDLE\n");
    sm->current_state = STATE_IDLE;
}

void handle_complete_other(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error: Invalid event for COMPLETE state\n");
}

void handle_error_reset(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Transition: ERROR -> IDLE\n");
    sm->current_state = STATE_IDLE;
}

void handle_error_other(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error: Invalid event for ERROR state\n");
}

void initialize_handlers(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    for (int s = 0; s < STATE_COUNT; s++) {
        for (int e = 0; e < EVENT_COUNT; e++) {
            sm->handlers[s][e] = NULL;
        }
    }
    
    sm->handlers[STATE_IDLE][EVENT_START] = handle_idle_start;
    sm->handlers[STATE_IDLE][EVENT_DATA] = handle_idle_other;
    sm->handlers[STATE_IDLE][EVENT_PROCESS] = handle_idle_other;
    sm->handlers[STATE_IDLE][EVENT_SUCCESS] = handle_idle_other;
    sm->handlers[STATE_IDLE][EVENT_FAILURE] = handle_idle_other;
    sm->handlers[STATE_IDLE][EVENT_RESET] = handle_idle_other;
    
    sm->handlers[STATE_READY][EVENT_START] = handle_ready_other;
    sm->handlers[STATE_READY][EVENT_DATA] = handle_ready_data;
    sm->handlers[STATE_READY][EVENT_PROCESS] = handle_ready_other;
    sm->handlers[STATE_READY][EVENT_SUCCESS] = handle_ready_other;
    sm->handlers[STATE_READY][EVENT_FAILURE] = handle_ready_other;
    sm->handlers[STATE_READY][EVENT_RESET] = handle_ready_other;
    
    sm->handlers[STATE_PROCESSING][EVENT_START] = handle_processing_other;
    sm->handlers[STATE_PROCESSING][EVENT_DATA] = handle_processing_other;
    sm->handlers[STATE_PROCESSING][EVENT_PROCESS] = handle_processing_process;
    sm->handlers[STATE_PROCESSING][EVENT_SUCCESS] = handle_processing_success;
    sm->handlers[STATE_PROCESSING][EVENT_FAILURE] = handle_processing_failure;
    sm->handlers[STATE_PROCESSING][EVENT_RESET] = handle_processing_other;
    
    sm->handlers[STATE_COMPLETE][EVENT_START] = handle_complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_DATA] = handle_complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_PROCESS] = handle_complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_SUCCESS] = handle_complete_other;
    sm->hand