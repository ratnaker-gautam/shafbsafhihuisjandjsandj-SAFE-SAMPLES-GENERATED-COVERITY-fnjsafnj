//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(void);

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

State current_state = STATE_IDLE;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
    printf("Enter command (start/reset/quit): ");
}

void handle_ready(void) {
    printf("State: READY - Ready to receive data\n");
    printf("Enter command (data/reset/quit): ");
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data\n");
    printf("Enter command (success/failure/reset/quit): ");
}

void handle_complete(void) {
    printf("State: COMPLETE - Operation successful\n");
    printf("Enter command (reset/quit): ");
}

void handle_error(void) {
    printf("State: ERROR - Operation failed\n");
    printf("Enter command (reset/quit): ");
}

int validate_event_input(const char* input, Event* event) {
    if (input == NULL || event == NULL) return 0;
    
    if (strcmp(input, "start") == 0) *event = EVENT_START;
    else if (strcmp(input, "data") == 0) *event = EVENT_DATA;
    else if (strcmp(input, "process") == 0) *event = EVENT_PROCESS;
    else if (strcmp(input, "success") == 0) *event = EVENT_SUCCESS;
    else if (strcmp(input, "failure") == 0) *event = EVENT_FAILURE;
    else if (strcmp(input, "reset") == 0) *event = EVENT_RESET;
    else if (strcmp(input, "quit") == 0) return 2;
    else return 0;
    
    return 1;
}

void transition_state(Event event) {
    State next_state = current_state;
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) next_state = STATE_READY;
            break;
        case STATE_READY:
            if (event == EVENT_DATA) next_state = STATE_PROCESSING;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) next_state = STATE_COMPLETE;
            else if (event == EVENT_FAILURE) next_state = STATE_ERROR;
            break;
        case STATE_COMPLETE:
        case STATE_ERROR:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        default:
            break;
    }
    
    if (next_state != current_state) {
        current_state = next_state;
    }
}

int main(void) {
    char input[32];
    Event event;
    int result;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data, process, success, failure, reset, quit\n");
    
    while (1) {
        state_handlers[current_state]();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        result = validate_event_input(input, &event);
        if (result == 2) {
            printf("Exiting state machine\n");
            break;
        } else if (result == 1) {
            transition_state(event);
        } else {
            printf("Invalid command: %s\n", input);
        }
    }
    
    return 0;
}