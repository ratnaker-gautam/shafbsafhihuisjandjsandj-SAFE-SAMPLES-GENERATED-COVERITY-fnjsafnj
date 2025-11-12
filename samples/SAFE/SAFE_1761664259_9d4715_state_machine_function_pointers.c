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

void idle_handler(void) {
    printf("State: IDLE - Waiting for start event\n");
}

void ready_handler(void) {
    printf("State: READY - Ready to receive data\n");
}

void processing_handler(void) {
    printf("State: PROCESSING - Processing data\n");
}

void complete_handler(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void error_handler(void) {
    printf("State: ERROR - Operation failed\n");
}

static State current_state = STATE_IDLE;

static StateHandler state_handlers[STATE_COUNT] = {
    idle_handler,
    ready_handler,
    processing_handler,
    complete_handler,
    error_handler
};

static State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,    STATE_IDLE,    STATE_IDLE,    STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY, STATE_READY, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    State next_state = transition_table[current_state][event];
    if (next_state < 0 || next_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    current_state = next_state;
    state_handlers[current_state]();
}

int get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event (0-6): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    
    if (choice < 0 || choice > 6) {
        return -1;
    }
    
    return choice;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Initial state: IDLE\n");
    
    state_handlers[current_state]();
    
    while (1) {
        int event_choice = get_user_event();
        
        if (event_choice == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (event_choice == 6) {
            printf("Exiting state machine.\n");
            break;
        }
        
        process_event((Event)event_choice);
    }
    
    return 0;
}