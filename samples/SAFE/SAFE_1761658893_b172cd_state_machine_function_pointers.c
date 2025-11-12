//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_COMPLETED,
    STATE_ERROR,
    NUM_STATES
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef void (*StateHandler)(void);

void handle_idle(void);
void handle_active(void);
void handle_paused(void);
void handle_completed(void);
void handle_error(void);

StateHandler state_table[NUM_STATES][NUM_EVENTS] = {
    {handle_active, NULL, NULL, NULL, handle_error, NULL},
    {NULL, handle_paused, NULL, handle_completed, handle_error, handle_idle},
    {NULL, NULL, handle_active, handle_completed, handle_error, handle_idle},
    {NULL, NULL, NULL, NULL, NULL, handle_idle},
    {NULL, NULL, NULL, NULL, NULL, handle_idle}
};

State current_state = STATE_IDLE;
int counter = 0;

void handle_idle(void) {
    printf("State: IDLE\n");
    counter = 0;
    current_state = STATE_IDLE;
}

void handle_active(void) {
    if (counter < 100) {
        counter++;
    }
    printf("State: ACTIVE, Counter: %d\n", counter);
    current_state = STATE_ACTIVE;
}

void handle_paused(void) {
    printf("State: PAUSED, Counter: %d\n", counter);
    current_state = STATE_PAUSED;
}

void handle_completed(void) {
    printf("State: COMPLETED, Final Counter: %d\n", counter);
    current_state = STATE_COMPLETED;
}

void handle_error(void) {
    printf("State: ERROR\n");
    current_state = STATE_ERROR;
}

int get_user_input(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (value < 0 || value >= NUM_EVENTS) {
        return -1;
    }
    
    return (int)value;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - PAUSE\n");
    printf("2 - RESUME\n");
    printf("3 - COMPLETE\n");
    printf("4 - ERROR\n");
    printf("5 - RESET\n");
    printf("Enter event number: ");
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        print_menu();
        
        int event = get_user_input();
        if (event == -1) {
            printf("Invalid input. Please enter a number between 0 and %d.\n", NUM_EVENTS - 1);
            continue;
        }
        
        if (event == EVENT_RESET && current_state == STATE_IDLE) {
            printf("Already in IDLE state.\n");
            continue;
        }
        
        StateHandler handler = state_table[current_state][event];
        if (handler == NULL) {
            printf("Invalid transition from current state.\n");
            continue;
        }
        
        handler();
        
        if (current_state == STATE_COMPLETED) {
            printf("Process completed. Exiting.\n");
            break;
        }
    }
    
    return 0;
}