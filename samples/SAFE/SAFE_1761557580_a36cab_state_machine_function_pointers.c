//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            printf("Transition: PROCESSING -> COMPLETE\n");
            current_state = STATE_COMPLETE;
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

void handle_complete(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
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

StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

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
    
    printf("State Machine Demo\n");
    printf("Available events: start, data, process, success, failure, reset\n");
    printf("Type 'quit' to exit\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        printf("\nEnter event: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        Event event = get_event_from_input(input);
        if (event < 0 || event > EVENT_RESET) {
            printf("Invalid event: %s\n", input);
            continue;
        }
        
        if (current_state < 0 || current_state > STATE_ERROR) {
            printf("Invalid current state\n");
            current_state = STATE_IDLE;
            continue;
        }
        
        state_handlers[current_state](event);
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");
    }
    
    printf("Exiting state machine\n");
    return 0;
}