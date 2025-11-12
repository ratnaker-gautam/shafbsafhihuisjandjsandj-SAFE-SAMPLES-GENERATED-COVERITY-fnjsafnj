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

State handle_idle(Event evt);
State handle_reading(Event evt);
State handle_processing(Event evt);
State handle_writing(Event evt);
State handle_error(Event evt);

State handle_idle(Event evt) {
    switch (evt) {
        case EVENT_START:
            printf("Transition: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_reading(Event evt) {
    switch (evt) {
        case EVENT_DATA_READY:
            printf("Transition: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        case EVENT_ERROR:
            printf("Transition: READING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: READING -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_READING;
    }
}

State handle_processing(Event evt) {
    switch (evt) {
        case EVENT_PROCESS_DONE:
            printf("Transition: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Transition: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_writing(Event evt) {
    switch (evt) {
        case EVENT_WRITE_DONE:
            printf("Transition: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Transition: WRITING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: WRITING -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_WRITING;
    }
}

State handle_error(Event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
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

int main(void) {
    State current_state = STATE_IDLE;
    char input[32];
    int event_choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    printf("Enter event number (0-%d) or -1 to quit:\n", EVENT_COUNT - 1);
    
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
            printf("Invalid event number. Must be between 0 and %d.\n", EVENT_COUNT - 1);
            continue;
        }
        
        Event evt = (Event)event_choice;
        State new_state = state_handlers[current_state](evt);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            break;
        }
        
        current_state = new_state;
    }
    
    printf("State machine terminated.\n");
    return 0;
}