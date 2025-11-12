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
    printf("State: IDLE - Waiting for start command\n");
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

StateHandler state_handlers[STATE_COUNT] = {
    idle_handler,
    ready_handler,
    processing_handler,
    complete_handler,
    error_handler
};

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_IDLE, STATE_READY, STATE_IDLE, STATE_IDLE, STATE_IDLE, STATE_IDLE},
    {STATE_READY, STATE_READY, STATE_PROCESSING, STATE_READY, STATE_READY, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

int get_user_input(void) {
    char input[32];
    printf("Enter event (0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (value < 0 || value > 6) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (running) {
        if (current_state >= STATE_COUNT || current_state < 0) {
            current_state = STATE_IDLE;
        }
        
        state_handlers[current_state]();
        
        int event_input = get_user_input();
        
        if (event_input == 6) {
            running = 0;
            continue;
        }
        
        if (event_input == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        Event event = (Event)event_input;
        
        if (event >= EVENT_COUNT || event < 0) {
            printf("Invalid event. Please try again.\n");
            continue;
        }
        
        State next_state = transition_table[current_state][event];
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            next_state = STATE_IDLE;
        }
        
        current_state = next_state;
    }
    
    printf("State machine terminated.\n");
    return 0;
}