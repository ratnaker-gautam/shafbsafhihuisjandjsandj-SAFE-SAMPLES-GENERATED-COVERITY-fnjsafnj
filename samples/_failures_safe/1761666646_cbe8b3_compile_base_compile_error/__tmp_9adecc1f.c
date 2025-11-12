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
    EVENT_QUIT,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    StateHandler handlers[STATE_COUNT][EVENT_COUNT];
    int data_value;
    int error_code;
};

void handle_idle_start(void) {
    printf("Transition: IDLE -> READY\n");
}

void handle_idle_other(void) {
    printf("Error: Invalid event for IDLE state\n");
}

void handle_ready_data(void) {
    printf("Transition: READY -> PROCESSING\n");
}

void handle_ready_other(void) {
    printf("Error: Invalid event for READY state\n");
}

void handle_processing_process(void) {
    printf("Processing data...\n");
}

void handle_processing_success(void) {
    printf("Transition: PROCESSING -> COMPLETE\n");
}

void handle_processing_failure(void) {
    printf("Transition: PROCESSING -> ERROR\n");
}

void handle_processing_other(void) {
    printf("Error: Invalid event for PROCESSING state\n");
}

void handle_complete_reset(void) {
    printf("Transition: COMPLETE -> IDLE\n");
}

void handle_complete_other(void) {
    printf("Error: Invalid event for COMPLETE state\n");
}

void handle_error_reset(void) {
    printf("Transition: ERROR -> IDLE\n");
}

void handle_error_other(void) {
    printf("Error: Invalid event for ERROR state\n");
}

void init_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
    
    for (int s = 0; s < STATE_COUNT; s++) {
        for (int e = 0; e < EVENT_COUNT; e++) {
            sm->handlers[s][e] = NULL;
        }
    }
    
    sm->handlers[STATE_IDLE][EVENT_START] = handle_idle_start;
    sm->handlers[STATE_READY][EVENT_DATA] = handle_ready_data;
    sm->handlers[STATE_PROCESSING][EVENT_PROCESS] = handle_processing_process;
    sm->handlers[STATE_PROCESSING][EVENT_SUCCESS] = handle_processing_success;
    sm->handlers[STATE_PROCESSING][EVENT_FAILURE] = handle_processing_failure;
    sm->handlers[STATE_COMPLETE][EVENT_RESET] = handle_complete_reset;
    sm->handlers[STATE_ERROR][EVENT_RESET] = handle_error_reset;
    
    for (int e = 0; e < EVENT_COUNT; e++) {
        if (sm->handlers[STATE_IDLE][e] == NULL) {
            sm->handlers[STATE_IDLE][e] = handle_idle_other;
        }
        if (sm->handlers[STATE_READY][e] == NULL) {
            sm->handlers[STATE_READY][e] = handle_ready_other;
        }
        if (sm->handlers[STATE_PROCESSING][e] == NULL) {
            sm->handlers[STATE_PROCESSING][e] = handle_processing_other;
        }
        if (sm->handlers[STATE_COMPLETE][e] == NULL) {
            sm->handlers[STATE_COMPLETE][e] = handle_complete_other;
        }
        if (sm->handlers[STATE_ERROR][e] == NULL) {
            sm->handlers[STATE_ERROR][e] = handle_error_other;
        }
    }
}

int process_event(struct StateMachine *sm, Event event) {
    if (sm == NULL) return -1;
    if (event < 0 || event >= EVENT_COUNT) return -1;
    
    StateHandler handler = sm->handlers[sm->current_state][event];
    if (handler != NULL) {
        handler();
        return 0;
    }
    return -1;
}

void update_state(struct StateMachine *sm, State new_state) {
    if (sm == NULL) return;
    if (new_state < 0 || new_state >= STATE_COUNT) return;
    sm->current_state = new_state;
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    int running = 1;
    char input[32];
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT\n");
    
    while (running) {
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR");