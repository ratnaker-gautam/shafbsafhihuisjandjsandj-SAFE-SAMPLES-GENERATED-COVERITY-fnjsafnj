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

static StateHandler state_handlers[STATE_COUNT] = {
    idle_handler,
    ready_handler,
    processing_handler,
    complete_handler,
    error_handler
};

static State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,     STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

int get_valid_event(void) {
    int event;
    char input[32];
    
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &event) != 1) {
        return -1;
    }
    
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    return event;
}

int main(void) {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("Finite State Machine Demo\n");
    printf("Type 'quit' to exit\n\n");
    
    while (running) {
        state_handlers[current_state]();
        
        int event = get_valid_event();
        if (event == -1) {
            char input[32];
            if (fgets(input, sizeof(input), stdin) != NULL) {
                if (strncmp(input, "quit", 4) == 0) {
                    running = 0;
                }
            }
            continue;
        }
        
        State next_state = transition_table[current_state][event];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            continue;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_COMPLETE || current_state == STATE_ERROR) {
            printf("Final state reached. Use RESET to continue or quit to exit.\n");
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}