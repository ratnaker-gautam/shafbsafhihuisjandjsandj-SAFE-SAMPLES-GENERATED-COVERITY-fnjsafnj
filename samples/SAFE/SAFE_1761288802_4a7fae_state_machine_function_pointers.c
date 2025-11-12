//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_ERROR:
            printf("Transition: IDLE -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            printf("Transition: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        case EVENT_ERROR:
            printf("Transition: READING -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event event) {
    switch (event) {
        case EVENT_PROCESS_DONE:
            printf("Transition: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Transition: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Event event) {
    switch (event) {
        case EVENT_WRITE_DONE:
            printf("Transition: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Transition: WRITING -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_DONE",
    "WRITE_DONE",
    "ERROR",
    "RESET"
};

int get_valid_event() {
    char input[32];
    int event;
    
    printf("Available events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    
    printf("Enter event number (0-%d): ", EVENT_COUNT - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &event) != 1) {
        return -1;
    }
    
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    return event;
}

int main() {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Current state: %s\n", state_names[current_state]);
    
    while (running) {
        int event_num = get_valid_event();
        if (event_num == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        Event event = (Event)event_num;
        
        if (event == EVENT_RESET && current_state != STATE_ERROR) {
            printf("RESET event only valid in ERROR state\n");
            continue;
        }
        
        StateHandler handler = state_handlers[current_state];
        State new_state = handler(event);
        
        if (new_state != current_state) {
            current_state = new_state;
            printf("Current state: %s\n", state_names[current_state]);
        }
        
        if (current_state == STATE_IDLE && event == EVENT_WRITE_DONE) {
            printf("Operation completed successfully. Exiting.\n");
            running = 0;
        }
    }
    
    return 0;
}