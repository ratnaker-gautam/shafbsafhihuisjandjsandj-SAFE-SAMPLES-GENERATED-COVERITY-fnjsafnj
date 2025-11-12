//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_COMPLETED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> ACTIVE\n");
            return STATE_ACTIVE;
        case EVENT_ERROR:
            printf("Transition: IDLE -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_active(Event event) {
    switch (event) {
        case EVENT_PAUSE:
            printf("Transition: ACTIVE -> PAUSED\n");
            return STATE_PAUSED;
        case EVENT_COMPLETE:
            printf("Transition: ACTIVE -> COMPLETED\n");
            return STATE_COMPLETED;
        case EVENT_ERROR:
            printf("Transition: ACTIVE -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for ACTIVE state\n");
            return STATE_ACTIVE;
    }
}

State handle_paused(Event event) {
    switch (event) {
        case EVENT_RESUME:
            printf("Transition: PAUSED -> ACTIVE\n");
            return STATE_ACTIVE;
        case EVENT_ERROR:
            printf("Transition: PAUSED -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PAUSED state\n");
            return STATE_PAUSED;
    }
}

State handle_completed(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETED -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETED state\n");
            return STATE_COMPLETED;
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

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_active,
        handle_paused,
        handle_completed,
        handle_error
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        return handlers[state];
    }
    return NULL;
}

Event get_event_from_input(int input) {
    if (input >= 0 && input <= 5) {
        return (Event)input;
    }
    return EVENT_ERROR;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0: START\n");
    printf("1: PAUSE\n");
    printf("2: RESUME\n");
    printf("3: COMPLETE\n");
    printf("4: ERROR\n");
    printf("5: RESET\n");
    printf("-1: EXIT\n");
    printf("Choose an event: ");
}

int main(void) {
    State current_state = STATE_IDLE;
    int input;
    Event event;
    StateHandler handler;
    
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == -1) {
            printf("Exiting state machine.\n");
            break;
        }
        
        event = get_event_from_input(input);
        handler = get_handler(current_state);
        
        if (handler != NULL) {
            current_state = handler(event);
        } else {
            printf("Invalid state handler\n");
        }
        
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE\n"); break;
            case STATE_ACTIVE: printf("ACTIVE\n"); break;
            case STATE_PAUSED: printf("PAUSED\n"); break;
            case STATE_COMPLETED: printf("COMPLETED\n"); break;
            case STATE_ERROR: printf("ERROR\n"); break;
        }
    }
    
    return 0;
}