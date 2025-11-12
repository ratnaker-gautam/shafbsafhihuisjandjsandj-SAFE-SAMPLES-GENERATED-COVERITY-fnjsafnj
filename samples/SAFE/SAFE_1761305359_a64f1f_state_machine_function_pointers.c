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

typedef void (*state_handler_t)(void);

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

state_handler_t state_table[NUM_STATES][NUM_EVENTS] = {
    {handle_ready, NULL, NULL, NULL, NULL, handle_idle},
    {NULL, handle_processing, NULL, NULL, handle_error, handle_idle},
    {NULL, NULL, handle_complete, NULL, handle_error, handle_idle},
    {NULL, NULL, NULL, NULL, NULL, handle_idle},
    {NULL, NULL, NULL, NULL, NULL, handle_idle}
};

state_t current_state = STATE_IDLE;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start event\n");
    current_state = STATE_IDLE;
}

void handle_ready(void) {
    printf("State: READY - Ready to receive data\n");
    current_state = STATE_READY;
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing received data\n");
    current_state = STATE_PROCESSING;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing completed successfully\n");
    current_state = STATE_COMPLETE;
}

void handle_error(void) {
    printf("State: ERROR - An error occurred during processing\n");
    current_state = STATE_ERROR;
}

event_t get_user_event(void) {
    char input[32];
    int choice;
    
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return NUM_EVENTS;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        return NUM_EVENTS;
    }
    
    if (choice < 0 || choice > 6) {
        return NUM_EVENTS;
    }
    
    if (choice == 6) {
        return NUM_EVENTS;
    }
    
    return (event_t)choice;
}

void process_event(event_t event) {
    if (event < 0 || event >= NUM_EVENTS) {
        printf("Invalid event\n");
        return;
    }
    
    state_handler_t handler = state_table[current_state][event];
    if (handler != NULL) {
        handler();
    } else {
        printf("Invalid transition from current state\n");
    }
}

int main(void) {
    printf("Finite State Machine Demo\n");
    printf("Initial state: IDLE\n");
    
    while (1) {
        event_t event = get_user_event();
        if (event == NUM_EVENTS) {
            break;
        }
        
        process_event(event);
    }
    
    printf("State machine terminated\n");
    return 0;
}