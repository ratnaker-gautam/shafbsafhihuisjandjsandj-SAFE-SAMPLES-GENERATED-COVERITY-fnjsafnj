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

static state_handler_t state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

static const char* state_names[NUM_STATES] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

static const char* event_names[NUM_EVENTS] = {
    "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
};

state_t handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event %s for state IDLE\n", event_names[event]);
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
            printf("Invalid event %s for state READY\n", event_names[event]);
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
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event %s for state PROCESSING\n", event_names[event]);
            return STATE_PROCESSING;
    }
}

state_t handle_complete(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event %s for state COMPLETE\n", event_names[event]);
            return STATE_COMPLETE;
    }
}

state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event %s for state ERROR\n", event_names[event]);
            return STATE_ERROR;
    }
}

int main(void) {
    state_t current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT\n");
    
    while (running) {
        printf("Current state: %s\n", state_names[current_state]);
        printf("Enter event number: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 6) {
            running = 0;
            continue;
        }
        
        if (input < 0 || input >= NUM_EVENTS) {
            printf("Invalid event number. Must be between 0 and %d.\n", NUM_EVENTS - 1);
            continue;
        }
        
        event_t event = (event_t)input;
        state_t new_state = state_handlers[current_state](event);
        
        if (new_state >= 0 && new_state < NUM_STATES) {
            current_state = new_state;
        } else {
            printf("Error: Invalid state transition\n");
            current_state = STATE_ERROR;
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}