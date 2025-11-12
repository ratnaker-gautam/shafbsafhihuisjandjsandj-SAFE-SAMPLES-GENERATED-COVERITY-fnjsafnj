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
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_ready(Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            printf("Transition: READY -> PROCESSING\n");
            return STATE_PROCESSING;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

State handle_processing(Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> COMPLETE\n");
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            printf("Transition: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_complete(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

State handle_error(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

StateHandler state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

const char* state_names[NUM_STATES] = {
    "IDLE",
    "READY",
    "PROCESSING",
    "COMPLETE",
    "ERROR"
};

const char* event_names[NUM_EVENTS] = {
    "START",
    "DATA_READY",
    "PROCESS",
    "SUCCESS",
    "FAILURE",
    "RESET"
};

int get_user_event() {
    char input[32];
    printf("\nAvailable events: ");
    for (int i = 0; i < NUM_EVENTS; i++) {
        printf("%d=%s ", i, event_names[i]);
    }
    printf("\nEnter event number (0-%d) or -1 to quit: ", NUM_EVENTS - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -2;
    }
    
    if (value < -1 || value >= NUM_EVENTS) {
        return -2;
    }
    
    return (int)value;
}

int main() {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Current state: %s\n", state_names[current_state]);
    
    while (running) {
        int event_num = get_user_event();
        
        if (event_num == -1) {
            printf("Exiting state machine\n");
            break;
        }
        
        if (event_num == -2) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        Event event = (Event)event_num;
        State new_state = state_handlers[current_state](event);
        
        if (new_state != current_state) {
            current_state = new_state;
        }
        
        printf("Current state: %s\n", state_names[current_state]);
    }
    
    return 0;
}