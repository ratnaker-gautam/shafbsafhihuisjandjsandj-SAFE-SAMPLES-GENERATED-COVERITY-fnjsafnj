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
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    int data_value;
    int process_count;
};

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

StateHandler state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int transition_table[NUM_STATES][NUM_EVENTS] = {
    {STATE_READY,   STATE_IDLE,     STATE_IDLE,     STATE_IDLE,     STATE_IDLE,     STATE_IDLE},
    {STATE_READY,   STATE_READY,    STATE_PROCESSING, STATE_READY,   STATE_ERROR,    STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,   STATE_ERROR,    STATE_ERROR,    STATE_ERROR,    STATE_ERROR,    STATE_IDLE}
};

struct StateMachine sm;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start event\n");
    printf("Enter 'start' to begin: ");
}

void handle_ready(void) {
    printf("State: READY - Provide data value (1-100): ");
    int value;
    if (scanf("%d", &value) == 1) {
        if (value >= 1 && value <= 100) {
            sm.data_value = value;
            printf("Data accepted: %d\n", value);
        } else {
            printf("Invalid data. Must be 1-100.\n");
        }
    } else {
        while (getchar() != '\n');
        printf("Invalid input.\n");
    }
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", sm.data_value);
    sm.process_count++;
    
    if (sm.data_value > 0 && sm.process_count < 1000) {
        int result = sm.data_value * 2;
        if (result > sm.data_value) {
            sm.data_value = result;
            printf("Processed result: %d\n", sm.data_value);
        }
    }
    
    if (sm.process_count >= 3) {
        printf("Processing complete after %d cycles\n", sm.process_count);
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Final result: %d\n", sm.data_value);
    printf("Enter 'reset' to start over: ");
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
    printf("Enter 'reset' to recover: ");
}

Event get_event_from_input(void) {
    char input[32];
    if (scanf("%31s", input) != 1) {
        return NUM_EVENTS;
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return NUM_EVENTS;
    
    printf("Unknown command. Valid: start, data, process, success, failure, reset, quit\n");
    return NUM_EVENTS;
}

void initialize_state_machine(void) {
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.process_count = 0;
}

int main(void) {
    initialize_state_machine();
    printf("State Machine Demo - Type 'quit' to exit\n");
    
    while (1) {
        state_handlers[sm.current_state]();
        
        Event event = get_event_from_input();
        if (event == NUM_EVENTS) {
            break;
        }
        
        if (event >= 0 && event < NUM_EVENTS && sm.current_state >= 0 && sm.current_state < NUM_STATES) {
            State new_state = transition_table[sm.current_state][event];
            if (new_state >= 0 && new_state < NUM_STATES) {
                if (new_state != sm.current_state) {
                    if (new_state == STATE_IDLE) {
                        initialize_state_machine();
                    }
                    sm.current_state = new_state;
                }
            }
        }
        
        printf("\n");
    }
    
    printf("Exiting state machine demo.\n");
    return 0;
}