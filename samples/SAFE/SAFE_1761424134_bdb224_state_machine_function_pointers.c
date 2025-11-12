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

Event get_event_from_input(const char* input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "pause") == 0) return EVENT_PAUSE;
    if (strcmp(input, "resume") == 0) return EVENT_RESUME;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return -1;
}

void print_state_info(State state) {
    const char* state_names[] = {
        "IDLE", "ACTIVE", "PAUSED", "COMPLETED", "ERROR"
    };
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

void print_available_events(State state) {
    printf("Available events: ");
    switch (state) {
        case STATE_IDLE:
            printf("start, error");
            break;
        case STATE_ACTIVE:
            printf("pause, complete, error");
            break;
        case STATE_PAUSED:
            printf("resume, error");
            break;
        case STATE_COMPLETED:
            printf("reset");
            break;
        case STATE_ERROR:
            printf("reset");
            break;
        default:
            break;
    }
    printf("\n");
}

int main() {
    State current_state = STATE_IDLE;
    char input[32];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Type 'quit' to exit\n\n");
    
    while (running) {
        print_state_info(current_state);
        print_available_events(current_state);
        printf("Enter event: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        Event event = get_event_from_input(input);
        if (event == -1) {
            printf("Invalid event: %s\n", input);
            continue;
        }
        
        StateHandler handler = get_handler(current_state);
        if (handler != NULL) {
            State new_state = handler(event);
            if (new_state != current_state) {
                current_state = new_state;
            }
        }
        
        printf("\n");
    }
    
    printf("Exiting state machine demo\n");
    return 0;
}