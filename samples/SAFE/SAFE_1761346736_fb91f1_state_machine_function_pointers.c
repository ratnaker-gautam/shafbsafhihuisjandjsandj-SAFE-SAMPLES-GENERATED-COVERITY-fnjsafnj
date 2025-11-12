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
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} event_t;

typedef state_t (*state_handler_t)(event_t);

state_t handle_idle(event_t event);
state_t handle_ready(event_t event);
state_t handle_processing(event_t event);
state_t handle_complete(event_t event);
state_t handle_error(event_t event);

state_handler_t state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

state_t handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

state_t handle_ready(event_t event) {
    switch (event) {
        case EVENT_DATA:
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

state_t handle_processing(event_t event) {
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

state_t handle_complete(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

int main(void) {
    state_t current_state = STATE_IDLE;
    char input[32];
    int event_num;
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event number (0-5) or -1 to quit:\n");
    
    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n> ");
        
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
            printf("Invalid event number. Must be 0-5.\n");
            continue;
        }
        
        if (current_state >= 0 && current_state < NUM_STATES) {
            current_state = state_handlers[current_state]((event_t)event_num);
        } else {
            printf("Invalid state detected. Resetting to IDLE.\n");
            current_state = STATE_IDLE;
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}