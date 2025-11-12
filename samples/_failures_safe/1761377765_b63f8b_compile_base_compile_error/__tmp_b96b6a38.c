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

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    StateHandler handlers[STATE_COUNT][EVENT_COUNT];
};

void idle_start(void) {
    printf("Transition: IDLE -> READY\n");
}

void idle_other(void) {
    printf("Invalid event for IDLE state\n");
}

void ready_data(void) {
    printf("Transition: READY -> PROCESSING\n");
}

void ready_other(void) {
    printf("Invalid event for READY state\n");
}

void processing_process(void) {
    printf("Transition: PROCESSING -> COMPLETE\n");
}

void processing_failure(void) {
    printf("Transition: PROCESSING -> ERROR\n");
}

void processing_other(void) {
    printf("Invalid event for PROCESSING state\n");
}

void complete_reset(void) {
    printf("Transition: COMPLETE -> IDLE\n");
}

void complete_other(void) {
    printf("Invalid event for COMPLETE state\n");
}

void error_reset(void) {
    printf("Transition: ERROR -> IDLE\n");
}

void error_other(void) {
    printf("Invalid event for ERROR state\n");
}

void init_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    
    for (int s = 0; s < STATE_COUNT; s++) {
        for (int e = 0; e < EVENT_COUNT; e++) {
            sm->handlers[s][e] = NULL;
        }
    }
    
    sm->handlers[STATE_IDLE][EVENT_START] = idle_start;
    sm->handlers[STATE_IDLE][EVENT_DATA] = idle_other;
    sm->handlers[STATE_IDLE][EVENT_PROCESS] = idle_other;
    sm->handlers[STATE_IDLE][EVENT_SUCCESS] = idle_other;
    sm->handlers[STATE_IDLE][EVENT_FAILURE] = idle_other;
    sm->handlers[STATE_IDLE][EVENT_RESET] = idle_other;
    
    sm->handlers[STATE_READY][EVENT_START] = ready_other;
    sm->handlers[STATE_READY][EVENT_DATA] = ready_data;
    sm->handlers[STATE_READY][EVENT_PROCESS] = ready_other;
    sm->handlers[STATE_READY][EVENT_SUCCESS] = ready_other;
    sm->handlers[STATE_READY][EVENT_FAILURE] = ready_other;
    sm->handlers[STATE_READY][EVENT_RESET] = ready_other;
    
    sm->handlers[STATE_PROCESSING][EVENT_START] = processing_other;
    sm->handlers[STATE_PROCESSING][EVENT_DATA] = processing_other;
    sm->handlers[STATE_PROCESSING][EVENT_PROCESS] = processing_process;
    sm->handlers[STATE_PROCESSING][EVENT_SUCCESS] = processing_other;
    sm->handlers[STATE_PROCESSING][EVENT_FAILURE] = processing_failure;
    sm->handlers[STATE_PROCESSING][EVENT_RESET] = processing_other;
    
    sm->handlers[STATE_COMPLETE][EVENT_START] = complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_DATA] = complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_PROCESS] = complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_SUCCESS] = complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_FAILURE] = complete_other;
    sm->handlers[STATE_COMPLETE][EVENT_RESET] = complete_reset;
    
    sm->handlers[STATE_ERROR][EVENT_START] = error_other;
    sm->handlers[STATE_ERROR][EVENT_DATA] = error_other;
    sm->handlers[STATE_ERROR][EVENT_PROCESS] = error_other;
    sm->handlers[STATE_ERROR][EVENT_SUCCESS] = error_other;
    sm->handlers[STATE_ERROR][EVENT_FAILURE] = error_other;
    sm->handlers[STATE_ERROR][EVENT_RESET] = error_reset;
}

int handle_event(struct StateMachine *sm, Event event) {
    if (sm == NULL) return 0;
    if (event < 0 || event >= EVENT_COUNT) return 0;
    
    StateHandler handler = sm->handlers[sm->current_state][event];
    if (handler != NULL) {
        handler();
        
        switch (sm->current_state) {
            case STATE_IDLE:
                if (event == EVENT_START) sm->current_state = STATE_READY;
                break;
            case STATE_READY:
                if (event == EVENT_DATA) sm->current_state = STATE_PROCESSING;
                break;
            case STATE_PROCESSING:
                if (event == EVENT_PROCESS) sm->current_state =