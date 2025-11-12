//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

State handle_idle(Event evt);
State handle_reading(Event evt);
State handle_processing(Event evt);
State handle_writing(Event evt);
State handle_error(Event evt);

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

State handle_idle(Event evt) {
    if (evt == EVENT_START) {
        printf("Transition: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event evt) {
    if (evt == EVENT_DATA_READY) {
        printf("Transition: READING -> PROCESSING\n");
        return STATE_PROCESSING;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: READING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event evt) {
    if (evt == EVENT_PROCESS_COMPLETE) {
        printf("Transition: PROCESSING -> WRITING\n");
        return STATE_WRITING;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event evt) {
    if (evt == EVENT_WRITE_COMPLETE) {
        printf("Transition: WRITING -> IDLE\n");
        return STATE_IDLE;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: WRITING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event evt) {
    if (evt == EVENT_RESET) {
        printf("Transition: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int get_user_input(void) {
    char input[32];
    int choice = -1;
    
    printf("\nAvailable events:\n");
    printf("0: START\n");
    printf("1: DATA_READY\n");
    printf("2: PROCESS_COMPLETE\n");
    printf("3: WRITE_COMPLETE\n");
    printf("4: ERROR\n");
    printf("5: RESET\n");
    printf("6: EXIT\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 0 || choice > 6) {
        return -1;
    }
    
    return choice;
}

int main(void) {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Current state: %s\n", state_names[current_state]);
    
    while (running) {
        int choice = get_user_input();
        
        if (choice == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (choice == 6) {
            printf("Exiting state machine.\n");
            break;
        }
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event selection.\n");
            continue;
        }
        
        Event evt = (Event)choice;
        StateHandler handler = state_handlers[current_state];
        
        if (handler == NULL) {
            printf("Error: No handler for state %d\n", current_state);
            break;
        }
        
        State new_state = handler(evt);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Error: Invalid state transition\n");
            break;
        }
        
        current_state = new_state;
        printf("Current state: %s\n", state_names[current_state]);
    }
    
    return 0;
}