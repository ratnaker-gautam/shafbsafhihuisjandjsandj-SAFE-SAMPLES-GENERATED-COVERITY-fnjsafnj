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

static State current_state = STATE_IDLE;
static int data_value = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Ready to receive data\n");
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
}

void handle_complete(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void handle_error(void) {
    printf("State: ERROR - Operation failed\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int validate_event(Event event) {
    return event >= 0 && event < EVENT_COUNT;
}

int validate_state(State state) {
    return state >= 0 && state < STATE_COUNT;
}

void process_event(Event event) {
    if (!validate_event(event)) {
        printf("Invalid event\n");
        return;
    }
    
    if (!validate_state(current_state)) {
        printf("Invalid current state\n");
        return;
    }
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                current_state = STATE_READY;
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                data_value = 42;
                current_state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                current_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                current_state = STATE_ERROR;
            } else if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
            }
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
            }
            break;
        default:
            break;
    }
    
    if (validate_state(current_state)) {
        state_handlers[current_state]();
    }
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event numbers (0-5), -1 to exit:\n");
    
    int input;
    while (1) {
        printf("> ");
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input == -1) {
            break;
        }
        
        if (input >= 0 && input < EVENT_COUNT) {
            process_event((Event)input);
        } else {
            printf("Invalid event number\n");
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}