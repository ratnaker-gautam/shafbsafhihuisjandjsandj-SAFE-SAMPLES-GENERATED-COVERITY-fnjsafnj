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
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

state_t handle_idle(event_t event);
state_t handle_ready(event_t event);
state_t handle_processing(event_t event);
state_t handle_complete(event_t event);
state_t handle_error(event_t event);

state_handler_t handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

const char* event_names[EVENT_COUNT] = {
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

int get_user_event(void) {
    char input[32];
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Current state: ");
    printf("Enter event number (0-5) or -1 to exit: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -2;
    }
    
    if (value < -1 || value >= EVENT_COUNT) {
        return -2;
    }
    
    return (int)value;
}

int main(void) {
    state_t current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    while (running) {
        int event_num = get_user_event();
        
        if (event_num == -1) {
            running = 0;
            continue;
        }
        
        if (event_num < 0 || event_num >= EVENT_COUNT) {
            printf("Invalid event. Please try again.\n");
            continue;
        }
        
        event_t event = (event_t)event_num;
        state_t new_state = handlers[current_state](event);
        
        if (new_state != current_state) {
            printf("State changed from %s to %s\n", 
                   state_names[current_state], state_names[new_state]);
            current_state = new_state;
        } else {
            printf("Event %s ignored in state %s\n", 
                   event_names[event], state_names[current_state]);
        }
    }
    
    printf("State Machine Demo Ended\n");
    return 0;
}