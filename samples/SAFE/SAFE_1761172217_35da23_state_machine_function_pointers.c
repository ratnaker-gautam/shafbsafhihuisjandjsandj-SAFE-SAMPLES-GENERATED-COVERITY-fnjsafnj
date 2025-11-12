//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
};

typedef void (*StateHandler)(void);

void idle_handler(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void ready_handler(void) {
    printf("State: READY - Data loaded, ready to process\n");
}

void processing_handler(void) {
    printf("State: PROCESSING - Performing calculations\n");
}

void complete_handler(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void error_handler(void) {
    printf("State: ERROR - Operation failed\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    idle_handler,
    ready_handler,
    processing_handler,
    complete_handler,
    error_handler
};

int transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,     STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR,   STATE_ERROR,  STATE_ERROR,  STATE_IDLE}
};

int current_state = STATE_IDLE;

void process_event(int event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    int next_state = transition_table[current_state][event];
    if (next_state < 0 || next_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    current_state = next_state;
    
    if (state_handlers[current_state] != NULL) {
        state_handlers[current_state]();
    }
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA_READY\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose an event: ");
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Initial state: IDLE\n");
    
    state_handlers[current_state]();
    
    int running = 1;
    char input[32];
    
    while (running) {
        print_menu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice < 0 || choice > 6) {
            printf("Invalid choice. Please select 0-6.\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            printf("Exiting state machine.\n");
            continue;
        }
        
        process_event((int)choice);
    }
    
    return 0;
}