//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_ready(Event event) {
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

State handle_processing(Event event) {
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

State handle_complete(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

State handle_error(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

Event get_user_input(void) {
    char input[MAX_INPUT_LEN];
    
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number (0-5): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_FAILURE;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input format\n");
        return EVENT_FAILURE;
    }
    
    if (choice < 0 || choice > 5) {
        printf("Event number out of range\n");
        return EVENT_FAILURE;
    }
    
    return (Event)choice;
}

int main(void) {
    State current_state = STATE_IDLE;
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        Event event = get_user_input();
        
        if (event == EVENT_FAILURE && current_state != STATE_ERROR) {
            continue;
        }
        
        State new_state = handlers[current_state](event);
        
        if (new_state >= STATE_IDLE && new_state < MAX_STATES) {
            current_state = new_state;
        } else {
            printf("Invalid state transition\n");
            current_state = STATE_ERROR;
        }
        
        switch (current_state) {
            case STATE_IDLE:
                printf("Current state: IDLE\n");
                break;
            case STATE_READY:
                printf("Current state: READY\n");
                break;
            case STATE_PROCESSING:
                printf("Current state: PROCESSING\n");
                break;
            case STATE_COMPLETE:
                printf("Current state: COMPLETE\n");
                break;
            case STATE_ERROR:
                printf("Current state: ERROR\n");
                break;
        }
        
        if (current_state == STATE_COMPLETE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    return 0;
}