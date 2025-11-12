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
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
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
        case EVENT_PROCESS_COMPLETE:
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
        case EVENT_WRITE_COMPLETE:
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
    "PROCESS_COMPLETE",
    "WRITE_COMPLETE",
    "ERROR",
    "RESET"
};

int main() {
    State current_state = STATE_IDLE;
    char input[32];
    int event_choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    printf("Enter event number (0-%d) or -1 to exit:\n", EVENT_COUNT - 1);
    
    while (1) {
        printf("\nCurrent state: %s\n", state_names[current_state]);
        printf("Enter event: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &event_choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (event_choice == -1) {
            break;
        }
        
        if (event_choice < 0 || event_choice >= EVENT_COUNT) {
            printf("Invalid event. Please enter 0-%d or -1 to exit.\n", EVENT_COUNT - 1);
            continue;
        }
        
        Event event = (Event)event_choice;
        State new_state = state_handlers[current_state](event);
        
        if (new_state >= STATE_COUNT) {
            printf("Invalid state transition.\n");
            continue;
        }
        
        current_state = new_state;
    }
    
    printf("Exiting state machine.\n");
    return 0;
}