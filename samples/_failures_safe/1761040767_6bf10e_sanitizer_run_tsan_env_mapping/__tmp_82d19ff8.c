//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

struct StateMachine {
    State current_state;
    int data_value;
    int error_code;
};

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

struct StateMachine sm;

void handle_idle(void) {
    printf("State: IDLE\n");
    printf("Options: 0-Start, 5-Exit\n");
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Data value: %d\n", sm.data_value);
    printf("Options: 1-Data, 2-Process, 5-Exit\n");
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", sm.data_value);
    printf("Options: 3-Success, 4-Failure, 5-Exit\n");
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Successfully processed: %d\n", sm.data_value);
    printf("Options: 5-Reset, 6-Exit\n");
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Error code: %d\n", sm.error_code);
    printf("Options: 5-Reset, 6-Exit\n");
}

int validate_input(const char* input, int min, int max) {
    if (input == NULL) return 0;
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') return 0;
    if (value < min || value > max) return 0;
    
    return 1;
}

Event get_event_from_input(const char* input) {
    if (!validate_input(input, 0, 6)) return EVENT_COUNT;
    
    int value = atoi(input);
    switch(value) {
        case 0: return EVENT_START;
        case 1: return EVENT_DATA;
        case 2: return EVENT_PROCESS;
        case 3: return EVENT_SUCCESS;
        case 4: return EVENT_FAILURE;
        case 5: return EVENT_RESET;
        case 6: return EVENT_COUNT;
        default: return EVENT_COUNT;
    }
}

void transition(Event event) {
    State next_state = sm.current_state;
    
    switch(sm.current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) next_state = STATE_READY;
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                sm.data_value = 42;
                next_state = STATE_READY;
            } else if (event == EVENT_PROCESS) {
                next_state = STATE_PROCESSING;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                next_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                sm.error_code = 1;
                next_state = STATE_ERROR;
            }
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        default:
            break;
    }
    
    if (next_state != sm.current_state) {
        sm.current_state = next_state;
    }
}

int main(void) {
    char input[32];
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.error_code = 0;
    
    printf("State Machine Demo\n");
    printf("Enter option number to trigger events\n");
    
    while (1) {
        state_handlers[sm.current_state]();
        
        printf("Enter option: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        Event event = get_event_from_input(input);
        if (event == EVENT_COUNT) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (event == EVENT_RESET && sm.current_state == STATE_IDLE) {
            break;
        }
        
        transition(event);
        
        if (event == EVENT_COUNT) {
            break;
        }
    }
    
    printf("Exiting state machine.\n");
    return 0;
}