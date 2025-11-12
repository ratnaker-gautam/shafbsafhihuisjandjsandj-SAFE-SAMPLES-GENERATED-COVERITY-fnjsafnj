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

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Data loaded: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Working on data: %d\n", data_value);
    process_count++;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processed %d times\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - Something went wrong\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,    STATE_IDLE,    STATE_IDLE,    STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR,   STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR,   STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR,    STATE_IDLE}
};

int validate_event(int event) {
    return event >= 0 && event < EVENT_COUNT;
}

int validate_state(int state) {
    return state >= 0 && state < STATE_COUNT;
}

void process_event(int event) {
    if (!validate_event(event) || !validate_state(current_state)) {
        return;
    }
    
    State next_state = transition_table[current_state][event];
    
    if (!validate_state(next_state)) {
        return;
    }
    
    current_state = next_state;
    
    if (event == EVENT_START && current_state == STATE_READY) {
        data_value = 100;
    } else if (event == EVENT_DATA && current_state == STATE_READY) {
        data_value += 10;
    } else if (event == EVENT_PROCESS && current_state == STATE_PROCESSING) {
        data_value *= 2;
    } else if (event == EVENT_RESET && current_state == STATE_IDLE) {
        data_value = 0;
        process_count = 0;
    }
    
    state_handlers[current_state]();
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    int running = 1;
    int choice;
    
    printf("State Machine Demo Started\n");
    state_handlers[current_state]();
    
    while (running) {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event\n");
            continue;
        }
        
        process_event(choice);
    }
    
    printf("State Machine Demo Ended\n");
    return 0;
}