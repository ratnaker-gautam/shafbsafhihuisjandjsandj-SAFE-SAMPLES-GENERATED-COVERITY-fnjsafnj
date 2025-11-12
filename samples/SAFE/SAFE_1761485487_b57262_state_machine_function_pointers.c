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
    if (event == EVENT_START) {
        printf("Transition: IDLE -> READY\n");
        return STATE_READY;
    }
    return STATE_IDLE;
}

state_t handle_ready(event_t event) {
    if (event == EVENT_DATA) {
        printf("Transition: READY -> PROCESSING\n");
        return STATE_PROCESSING;
    } else if (event == EVENT_RESET) {
        printf("Transition: READY -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_READY;
}

state_t handle_processing(event_t event) {
    if (event == EVENT_SUCCESS) {
        printf("Transition: PROCESSING -> COMPLETE\n");
        return STATE_COMPLETE;
    } else if (event == EVENT_FAILURE) {
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    } else if (event == EVENT_RESET) {
        printf("Transition: PROCESSING -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_PROCESSING;
}

state_t handle_complete(event_t event) {
    if (event == EVENT_RESET) {
        printf("Transition: COMPLETE -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

state_t handle_error(event_t event) {
    if (event == EVENT_RESET) {
        printf("Transition: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int main(void) {
    state_t current_state = STATE_IDLE;
    char input[32];
    int event_num;
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_names[current_state]);
        printf("Enter event number: ");
        
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
            printf("Invalid event number. Must be between 0 and %d.\n", NUM_EVENTS - 1);
            continue;
        }
        
        state_handler_t handler = state_handlers[current_state];
        if (handler != NULL) {
            current_state = handler((event_t)event_num);
        }
        
        printf("\n");
    }
    
    printf("Exiting state machine.\n");
    return 0;
}