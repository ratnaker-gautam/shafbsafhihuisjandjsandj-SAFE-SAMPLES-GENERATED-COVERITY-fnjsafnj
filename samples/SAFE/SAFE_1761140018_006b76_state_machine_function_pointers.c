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
    NUM_STATES
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} event_t;

typedef state_t (*state_handler_t)(event_t);

state_t handle_idle(event_t event);
state_t handle_ready(event_t event);
state_t handle_processing(event_t event);
state_t handle_complete(event_t event);
state_t handle_error(event_t event);

state_handler_t state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

const char* state_names[NUM_STATES] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

const char* event_names[NUM_EVENTS] = {
    "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
};

state_t current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

state_t handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            printf("System starting up...\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

state_t handle_ready(event_t event) {
    switch (event) {
        case EVENT_DATA:
            data_value = 42;
            printf("Data received: %d\n", data_value);
            return STATE_READY;
        case EVENT_PROCESS:
            if (data_value != 0) {
                printf("Beginning data processing...\n");
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READY;
            }
        case EVENT_RESET:
            printf("Resetting system...\n");
            data_value = 0;
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

state_t handle_processing(event_t event) {
    switch (event) {
        case EVENT_SUCCESS:
            process_count++;
            printf("Processing successful. Count: %d\n", process_count);
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            printf("Processing failed\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

state_t handle_complete(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting after completion...\n");
            data_value = 0;
            process_count = 0;
            return STATE_IDLE;
        case EVENT_DATA:
            data_value = 64;
            printf("New data received: %d\n", data_value);
            return STATE_READY;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting after error...\n");
            data_value = 0;
            process_count = 0;
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

void process_event(event_t event) {
    if (event < 0 || event >= NUM_EVENTS) {
        printf("Invalid event code\n");
        return;
    }
    
    if (current_state < 0 || current_state >= NUM_STATES) {
        printf("Invalid state\n");
        return;
    }
    
    printf("Current state: %s, Event: %s\n", 
           state_names[current_state], event_names[event]);
    
    state_t new_state = state_handlers[current_state](event);
    
    if (new_state >= 0 && new_state < NUM_STATES) {
        current_state = new_state;
    } else {
        printf("Invalid state transition\n");
        current_state = STATE_ERROR;
    }
}

int main() {
    printf("State Machine System Started\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event number (0-5) or -1 to exit:\n");
    
    int input;
    while (1) {
        printf("> ");
        
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == -1) {
            break;
        }
        
        if (input < 0 || input >= NUM_EVENTS) {
            printf("Event must be between 0 and %d\n", NUM_EVENTS - 1);
            continue;
        }
        
        process_event((event_t)input);
    }
    
    printf("State Machine System Terminated\n");
    return 0;
}