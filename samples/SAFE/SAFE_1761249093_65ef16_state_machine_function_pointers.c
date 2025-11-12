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
static int process_count = 0;

void state_idle_handler(void) {
    printf("State: IDLE\n");
    printf("Options: 1-Start, 0-Exit\n");
}

void state_ready_handler(void) {
    printf("State: READY\n");
    printf("Options: 2-Enter Data, 3-Process, 6-Reset\n");
}

void state_processing_handler(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", data_value);
    printf("Options: 4-Success, 5-Failure, 6-Reset\n");
}

void state_complete_handler(void) {
    printf("State: COMPLETE\n");
    printf("Processed %d operations successfully\n", process_count);
    printf("Options: 6-Reset, 0-Exit\n");
}

void state_error_handler(void) {
    printf("State: ERROR\n");
    printf("Options: 6-Reset, 0-Exit\n");
}

static StateHandler state_handlers[STATE_COUNT] = {
    state_idle_handler,
    state_ready_handler,
    state_processing_handler,
    state_complete_handler,
    state_error_handler
};

int process_event(Event event) {
    State new_state = current_state;
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) new_state = STATE_READY;
            break;
        case STATE_READY:
            if (event == EVENT_DATA) new_state = STATE_READY;
            else if (event == EVENT_PROCESS) new_state = STATE_PROCESSING;
            else if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                new_state = STATE_COMPLETE;
                process_count++;
            } else if (event == EVENT_FAILURE) new_state = STATE_ERROR;
            else if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
        default:
            return 0;
    }
    
    if (new_state != current_state) {
        current_state = new_state;
        return 1;
    }
    return 0;
}

int get_valid_input(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (value < 0 || value > 6) return -1;
    
    return (int)value;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("==================\n");
    
    int running = 1;
    
    while (running) {
        state_handlers[current_state]();
        
        printf("Enter event (0-6): ");
        int input = get_valid_input();
        
        if (input == -1) {
            printf("Invalid input. Please enter a number 0-6.\n");
            continue;
        }
        
        Event event = (Event)input;
        
        if (event == 0) {
            running = 0;
            continue;
        }
        
        if (event == EVENT_DATA && current_state == STATE_READY) {
            printf("Enter data value: ");
            int data_input = get_valid_input();
            if (data_input != -1) {
                data_value = data_input;
                printf("Data set to: %d\n", data_value);
            } else {
                printf("Invalid data value.\n");
            }
            continue;
        }
        
        if (!process_event(event)) {
            printf("Invalid event for current state.\n");
        }
        
        printf("\n");
    }
    
    printf("Exiting state machine.\n");
    return 0;
}