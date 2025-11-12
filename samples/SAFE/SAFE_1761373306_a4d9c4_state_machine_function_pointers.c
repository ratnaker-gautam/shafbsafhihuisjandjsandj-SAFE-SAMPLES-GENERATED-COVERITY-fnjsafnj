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
    {STATE_READY, STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

State current_state = STATE_IDLE;

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return;
    }
    
    current_state = new_state;
    
    if (state_handlers[current_state] != NULL) {
        state_handlers[current_state]();
    }
}

Event get_user_event(void) {
    char input[32];
    printf("\nAvailable events:\n");
    printf("0 - START, 1 - DATA, 2 - PROCESS, 3 - SUCCESS, 4 - FAILURE, 5 - RESET\n");
    printf("Enter event number (0-5) or -1 to exit: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        return EVENT_COUNT;
    }
    
    if (value < -1 || value >= EVENT_COUNT) {
        return EVENT_COUNT;
    }
    
    return (Event)value;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        
        if (event == -1) {
            printf("Exiting state machine.\n");
            break;
        }
        
        if (event >= 0 && event < EVENT_COUNT) {
            process_event(event);
        } else {
            printf("Invalid event. Please try again.\n");
        }
    }
    
    return 0;
}