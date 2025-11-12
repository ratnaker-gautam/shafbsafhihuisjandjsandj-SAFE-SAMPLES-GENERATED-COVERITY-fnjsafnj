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
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event);
State handle_active(Event event);
State handle_paused(Event event);
State handle_completed(Event event);
State handle_error(Event event);

StateHandler handlers[] = {
    handle_idle,
    handle_active,
    handle_paused,
    handle_completed,
    handle_error
};

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
        case EVENT_STOP:
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
        case EVENT_STOP:
            printf("Transition: PAUSED -> COMPLETED\n");
            return STATE_COMPLETED;
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

int get_event_input(void) {
    char input[32];
    printf("Enter event (0=START, 1=PAUSE, 2=RESUME, 3=STOP, 4=ERROR, 5=RESET, -1=EXIT): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    int event;
    if (sscanf(input, "%d", &event) != 1) {
        return -2;
    }
    return event;
}

int main(void) {
    State current_state = STATE_IDLE;
    printf("State Machine Demo Started. Current state: IDLE\n");
    
    while (1) {
        int event_input = get_event_input();
        if (event_input == -1) {
            printf("Exiting state machine.\n");
            break;
        }
        if (event_input < 0 || event_input > 5) {
            printf("Invalid event number. Please enter 0-5 or -1 to exit.\n");
            continue;
        }
        
        Event event = (Event)event_input;
        StateHandler handler = handlers[current_state];
        State new_state = handler(event);
        
        if (new_state != current_state) {
            current_state = new_state;
        }
        
        const char* state_names[] = {"IDLE", "ACTIVE", "PAUSED", "COMPLETED", "ERROR"};
        printf("Current state: %s\n", state_names[current_state]);
    }
    
    return 0;
}