//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

void idle_handler(void) {
    printf("State: IDLE - Waiting for start event\n");
}

void ready_handler(void) {
    printf("State: READY - Ready to receive data\n");
}

void processing_handler(void) {
    printf("State: PROCESSING - Processing data\n");
}

void complete_handler(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void error_handler(void) {
    printf("State: ERROR - Operation failed\n");
}

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    [STATE_IDLE] = {
        [EVENT_START] = STATE_READY,
        [EVENT_DATA] = STATE_IDLE,
        [EVENT_PROCESS] = STATE_IDLE,
        [EVENT_SUCCESS] = STATE_IDLE,
        [EVENT_FAILURE] = STATE_IDLE,
        [EVENT_RESET] = STATE_IDLE
    },
    [STATE_READY] = {
        [EVENT_START] = STATE_READY,
        [EVENT_DATA] = STATE_PROCESSING,
        [EVENT_PROCESS] = STATE_READY,
        [EVENT_SUCCESS] = STATE_READY,
        [EVENT_FAILURE] = STATE_READY,
        [EVENT_RESET] = STATE_IDLE
    },
    [STATE_PROCESSING] = {
        [EVENT_START] = STATE_PROCESSING,
        [EVENT_DATA] = STATE_PROCESSING,
        [EVENT_PROCESS] = STATE_PROCESSING,
        [EVENT_SUCCESS] = STATE_COMPLETE,
        [EVENT_FAILURE] = STATE_ERROR,
        [EVENT_RESET] = STATE_IDLE
    },
    [STATE_COMPLETE] = {
        [EVENT_START] = STATE_COMPLETE,
        [EVENT_DATA] = STATE_COMPLETE,
        [EVENT_PROCESS] = STATE_COMPLETE,
        [EVENT_SUCCESS] = STATE_COMPLETE,
        [EVENT_FAILURE] = STATE_COMPLETE,
        [EVENT_RESET] = STATE_IDLE
    },
    [STATE_ERROR] = {
        [EVENT_START] = STATE_ERROR,
        [EVENT_DATA] = STATE_ERROR,
        [EVENT_PROCESS] = STATE_ERROR,
        [EVENT_SUCCESS] = STATE_ERROR,
        [EVENT_FAILURE] = STATE_ERROR,
        [EVENT_RESET] = STATE_IDLE
    }
};

StateHandler state_handlers[STATE_COUNT] = {
    [STATE_IDLE] = idle_handler,
    [STATE_READY] = ready_handler,
    [STATE_PROCESSING] = processing_handler,
    [STATE_COMPLETE] = complete_handler,
    [STATE_ERROR] = error_handler
};

typedef struct {
    State current_state;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
}

void state_machine_process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return;
    if (sm->current_state >= STATE_COUNT) return;
    if (event >= EVENT_COUNT) return;
    
    State next_state = transition_table[sm->current_state][event];
    if (next_state >= STATE_COUNT) return;
    
    sm->current_state = next_state;
    
    if (state_handlers[sm->current_state] != NULL) {
        state_handlers[sm->current_state]();
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    int running = 1;
    char input[32];
    
    printf("State Machine Demo\n");
    printf("Available events: start, data, process, success, failure, reset, quit\n");
    
    while (running) {
        printf("\nEnter event: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "data") == 0) {
            event = EVENT_DATA;
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
        } else if (strcmp(input, "success") == 0) {
            event = EVENT_SUCCESS;
        } else if (strcmp(input, "failure") == 0) {
            event = EVENT_FAILURE;
        } else if (strcmp(input, "reset") == 0) {
            event = EVENT_RESET;
        } else if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        } else {
            printf("Invalid event\n");
            continue;
        }
        
        state