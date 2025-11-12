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

static state_handler_t state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

static const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

static const char* event_names[EVENT_COUNT] = {
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
            printf("Data received, ready to process\n");
            return STATE_READY;
        case EVENT_PROCESS:
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
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter -1 to exit\n\n");
    
    while (running) {
        printf("Current state: %s\n", state_names[current_state]);
        printf("Enter event number: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == -1) {
            running = 0;
            continue;
        }
        
        if (input < 0 || input >= EVENT_COUNT) {
            printf("Invalid event number. Must be 0-%d or -1 to exit\n", EVENT_COUNT - 1);
            continue;
        }
        
        event_t event = (event_t)input;
        state_t new_state = state_handlers[current_state](event);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Error: Invalid state transition\n");
            continue;
        }
        
        current_state = new_state;
        printf("\n");
    }
    
    printf("State machine terminated\n");
    return 0;
}