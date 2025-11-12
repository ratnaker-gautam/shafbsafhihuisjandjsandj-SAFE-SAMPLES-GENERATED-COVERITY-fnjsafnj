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
    {STATE_READY,    STATE_IDLE, STATE_IDLE,      STATE_IDLE,    STATE_IDLE,    STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY,  STATE_ERROR,   STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR,    STATE_ERROR,    STATE_ERROR,    STATE_IDLE}
};

void process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    State next_state = transition_table[current_state][event];
    
    if (next_state < 0 || next_state >= STATE_COUNT) {
        return;
    }
    
    if (event == EVENT_DATA) {
        if (data >= 0 && data <= 100) {
            data_value = data;
        }
    }
    
    if (event == EVENT_RESET) {
        data_value = 0;
        process_count = 0;
    }
    
    current_state = next_state;
    
    if (state_handlers[current_state] != NULL) {
        state_handlers[current_state]();
    }
}

int main(void) {
    char input[32];
    int value;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <0-100>, process, success, failure, reset, quit\n");
    
    while (1) {
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "start") == 0) {
            process_event(EVENT_START, 0);
        } else if (strncmp(input, "data ", 5) == 0) {
            if (sscanf(input + 5, "%d", &value) == 1) {
                if (value >= 0 && value <= 100) {
                    process_event(EVENT_DATA, value);
                } else {
                    printf("Error: Data must be between 0 and 100\n");
                }
            } else {
                printf("Error: Invalid data format\n");
            }
        } else if (strcmp(input, "process") == 0) {
            process_event(EVENT_PROCESS, 0);
        } else if (strcmp(input, "success") == 0) {
            process_event(EVENT_SUCCESS, 0);
        } else if (strcmp(input, "failure") == 0) {
            process_event(EVENT_FAILURE, 0);
        } else if (strcmp(input, "reset") == 0) {
            process_event(EVENT_RESET, 0);
        } else {
            printf("Error: Unknown command\n");
        }
    }
    
    return 0;
}