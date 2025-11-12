//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_RESET,
    EVENT_ERROR,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            return STATE_READY;
        case EVENT_RESET:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(Event event) {
    switch (event) {
        case EVENT_START:
            return STATE_RUNNING;
        case EVENT_STOP:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READY;
    }
}

State handle_running(Event event) {
    switch (event) {
        case EVENT_PAUSE:
            return STATE_PAUSED;
        case EVENT_STOP:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_RUNNING;
    }
}

State handle_paused(Event event) {
    switch (event) {
        case EVENT_RESUME:
            return STATE_RUNNING;
        case EVENT_STOP:
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PAUSED;
    }
}

State handle_error(Event event) {
    switch (event) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_running,
    handle_paused,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READY",
    "RUNNING",
    "PAUSED",
    "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START",
    "PAUSE",
    "RESUME",
    "STOP",
    "RESET",
    "ERROR"
};

int get_event_input(void) {
    char input[32];
    printf("Available events: ");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d=%s ", i, event_names[i]);
    }
    printf("\nEnter event number (0-%d): ", EVENT_COUNT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (value < 0 || value >= EVENT_COUNT) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Initial state: %s\n", state_names[current_state]);
    
    while (running) {
        int event_num = get_event_input();
        if (event_num == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        Event event = (Event)event_num;
        StateHandler handler = state_handlers[current_state];
        State new_state = handler(event);
        
        printf("Event: %s -> State: %s -> %s\n", 
               event_names[event], 
               state_names[current_state], 
               state_names[new_state]);
        
        current_state = new_state;
        
        if (event == EVENT_STOP && current_state == STATE_IDLE) {
            running = 0;
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}