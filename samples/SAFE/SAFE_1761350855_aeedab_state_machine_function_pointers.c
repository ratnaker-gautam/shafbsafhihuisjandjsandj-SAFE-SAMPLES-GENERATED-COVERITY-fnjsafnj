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
    NUM_STATES
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
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

StateHandler state_table[NUM_STATES][NUM_EVENTS] = {
    {ready_handler, NULL, NULL, NULL, NULL, idle_handler},
    {NULL, processing_handler, NULL, NULL, error_handler, idle_handler},
    {NULL, NULL, complete_handler, NULL, error_handler, idle_handler},
    {NULL, NULL, NULL, NULL, NULL, idle_handler},
    {NULL, NULL, NULL, NULL, NULL, idle_handler}
};

State current_state = STATE_IDLE;

void process_event(Event event) {
    if (event < 0 || event >= NUM_EVENTS) {
        printf("Invalid event\n");
        return;
    }
    
    StateHandler handler = state_table[current_state][event];
    if (handler != NULL) {
        handler();
        
        switch (current_state) {
            case STATE_IDLE:
                if (event == EVENT_START) current_state = STATE_READY;
                break;
            case STATE_READY:
                if (event == EVENT_DATA) current_state = STATE_PROCESSING;
                else if (event == EVENT_FAILURE) current_state = STATE_ERROR;
                break;
            case STATE_PROCESSING:
                if (event == EVENT_PROCESS) current_state = STATE_COMPLETE;
                else if (event == EVENT_FAILURE) current_state = STATE_ERROR;
                break;
            case STATE_COMPLETE:
            case STATE_ERROR:
                if (event == EVENT_RESET) current_state = STATE_IDLE;
                break;
            default:
                break;
        }
    } else {
        printf("Event %d not allowed in current state %d\n", event, current_state);
    }
}

int main(void) {
    char input[32];
    int event_num;
    
    printf("State Machine Demo\n");
    printf("Events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event numbers (0-5) or -1 to quit:\n");
    
    while (1) {
        printf("Current state: %d | Enter event: ", current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &event_num) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (event_num == -1) {
            break;
        }
        
        if (event_num < 0 || event_num >= NUM_EVENTS) {
            printf("Event must be between 0 and %d\n", NUM_EVENTS - 1);
            continue;
        }
        
        process_event((Event)event_num);
    }
    
    printf("State machine terminated.\n");
    return 0;
}