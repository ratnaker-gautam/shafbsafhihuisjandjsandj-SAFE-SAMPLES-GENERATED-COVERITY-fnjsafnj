//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(void);

void handle_idle(void) {
    printf("System is idle. Waiting for start command.\n");
}

void handle_ready(void) {
    printf("System is ready. Preparing to run.\n");
}

void handle_running(void) {
    printf("System is running. Processing tasks.\n");
}

void handle_paused(void) {
    printf("System is paused. Waiting for resume or stop.\n");
}

void handle_stopped(void) {
    printf("System is stopped. Ready for reset.\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_running,
    handle_paused,
    handle_stopped
};

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,   STATE_IDLE,   STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,   STATE_READY,  STATE_RUNNING,STATE_STOPPED,STATE_IDLE},
    {STATE_RUNNING, STATE_PAUSED, STATE_RUNNING,STATE_STOPPED,STATE_IDLE},
    {STATE_PAUSED,  STATE_PAUSED, STATE_RUNNING,STATE_STOPPED,STATE_IDLE},
    {STATE_STOPPED, STATE_STOPPED,STATE_STOPPED,STATE_STOPPED,STATE_IDLE}
};

State current_state = STATE_IDLE;

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event.\n");
        return;
    }
    
    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) {
        printf("Invalid state transition.\n");
        return;
    }
    
    current_state = new_state;
    state_handlers[current_state]();
}

Event get_user_event(void) {
    char input[32];
    printf("\nAvailable events:\n");
    printf("0: START, 1: PAUSE, 2: RESUME, 3: STOP, 4: RESET\n");
    printf("Enter event number (0-4) or -1 to exit: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input. Please enter a number.\n");
        return -2;
    }
    
    if (value < -1 || value >= EVENT_COUNT) {
        printf("Invalid event number. Please enter 0-4 or -1 to exit.\n");
        return -2;
    }
    
    return (Event)value;
}

int main(void) {
    printf("State Machine Demo Started\n");
    printf("Initial State: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == -1) {
            printf("Exiting state machine.\n");
            break;
        }
        if (event == -2) {
            continue;
        }
        
        process_event(event);
    }
    
    return 0;
}