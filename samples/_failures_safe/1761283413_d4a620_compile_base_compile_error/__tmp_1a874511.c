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
int data_value = 0;
int result = 0;

void handle_idle(Event event) {
    switch (event) {
        case EVENT_START:
            current_state = STATE_READY;
            printf("State: IDLE -> READY\n");
            break;
        case EVENT_RESET:
            current_state = STATE_IDLE;
            printf("State: IDLE (reset)\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(Event event) {
    switch (event) {
        case EVENT_DATA:
            current_state = STATE_PROCESSING;
            printf("State: READY -> PROCESSING\n");
            break;
        case EVENT_RESET:
            current_state = STATE_IDLE;
            printf("State: READY -> IDLE\n");
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            current_state = STATE_COMPLETE;
            printf("State: PROCESSING -> COMPLETE\n");
            break;
        case EVENT_FAILURE:
            current_state = STATE_ERROR;
            printf("State: PROCESSING -> ERROR\n");
            break;
        case EVENT_RESET:
            current_state = STATE_IDLE;
            printf("State: PROCESSING -> IDLE\n");
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_complete(Event event) {
    switch (event) {
        case EVENT_RESET:
            current_state = STATE_IDLE;
            printf("State: COMPLETE -> IDLE\n");
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(Event event) {
    switch (event) {
        case EVENT_RESET:
            current_state = STATE_IDLE;
            printf("State: ERROR -> IDLE\n");
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
        case STATE_COMPLETE: return handle_complete;
        case STATE_ERROR: return handle_error;
        default: return NULL;
    }
}

Event get_user_event() {
    char input[32];
    printf("Enter event (start, data, process, success, failure, reset, quit): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_RESET;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return EVENT_RESET;
    
    printf("Unknown event\n");
    return EVENT_RESET;
}

void process_data() {
    if (current_state == STATE_PROCESSING) {
        printf("Processing data...\n");
        if (data_value >= 0 && data_value <= 100) {
            result = data_value * 2;
            printf("Result: %d\n", result);
        } else {
            printf("Invalid data value\n");
        }
    }
}

int main() {
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        
        if (event == EVENT_RESET && current_state == STATE_IDLE) {
            break;
        }
        
        StateHandler handler = get_handler(current_state);
        if (handler != NULL) {
            handler(event);
        }
        
        if (event == EVENT_DATA) {
            printf("Enter data value (0-100): ");
            char input[32];
            if (fgets(input, sizeof(input), stdin) != NULL) {
                char *endptr;
                long val = strtol(input, &endptr, 10);
                if (endptr != input && val >= 0 && val <= 100) {
                    data_value = (int)val;
                } else {
                    printf("Invalid input, using default 0\n");
                    data_value = 0;
                }
            }
        }
        
        if (event == EVENT_PROCESS) {
            process_data();
        }
        
        if (event == EVENT_SUCCESS && current_state == STATE_PROCESSING) {
            printf("Processing successful!\n");
            handle_processing(EVENT_SUCCESS);
        }
        
        if (event ==