//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
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

typedef void (*StateHandler)(Event);

State current_state = STATE_IDLE;

void handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            current_state = STATE_READY;
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(Event event) {
    switch (event) {
        case EVENT_DATA:
            printf("Transition: READY -> PROCESSING\n");
            current_state = STATE_PROCESSING;
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> DONE\n");
            current_state = STATE_DONE;
            break;
        case EVENT_FAILURE:
            printf("Transition: PROCESSING -> ERROR\n");
            current_state = STATE_ERROR;
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_done(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: DONE -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for DONE state\n");
            break;
    }
}

void handle_error(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

StateHandler get_handler(State state) {
    switch (state) {
        case STATE_IDLE: return handle_idle;
        case STATE_READY: return handle_ready;
        case STATE_PROCESSING: return handle_processing;
        case STATE_DONE: return handle_done;
        case STATE_ERROR: return handle_error;
        default: return NULL;
    }
}

Event get_event_from_input(const char* input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return -1;
}

int main() {
    char input[32];
    StateHandler handler;
    
    printf("State Machine Demo\n");
    printf("Available events: start, data, process, success, failure, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter event: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        Event event = get_event_from_input(input);
        if (event == -1) {
            printf("Invalid event: %s\n", input);
            continue;
        }
        
        handler = get_handler(current_state);
        if (handler != NULL) {
            handler(event);
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}