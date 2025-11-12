//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE,     STATE_IDLE,     STATE_IDLE,     STATE_IDLE,     STATE_IDLE},
    {STATE_READY,    STATE_READY,    STATE_PROCESSING, STATE_READY,   STATE_ERROR,    STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR,    STATE_ERROR,    STATE_ERROR,    STATE_ERROR,    STATE_IDLE}
};

State current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start event\n");
}

void handle_ready(void) {
    printf("State: READY - Data value: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (data_value > 0) {
        int result = data_value * 2;
        if (result < data_value) {
            current_state = STATE_ERROR;
            return;
        }
        data_value = result;
        process_count++;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Final value: %d, Processes: %d\n", data_value, process_count);
}

void handle_error(void) {
    printf("State: ERROR - Invalid operation detected\n");
}

Event get_user_event(void) {
    char input[32];
    printf("Enter event (0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return EVENT_COUNT;
    }
    
    if (value < 0 || value >= EVENT_COUNT) {
        return EVENT_COUNT;
    }
    
    return (Event)value;
}

int get_data_input(void) {
    char input[32];
    printf("Enter data value (0-1000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (value < 0 || value > 1000) {
        return -1;
    }
    
    return (int)value;
}

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    if (current_state < 0 || current_state >= STATE_COUNT) {
        current_state = STATE_ERROR;
        return;
    }
    
    State next_state = transition_table[current_state][event];
    
    if (next_state < 0 || next_state >= STATE_COUNT) {
        current_state = STATE_ERROR;
        return;
    }
    
    if (event == EVENT_DATA) {
        int new_data = get_data_input();
        if (new_data >= 0) {
            data_value = new_data;
        }
    }
    
    if (event == EVENT_PROCESS && data_value > 1000) {
        current_state = STATE_ERROR;
        return;
    }
    
    current_state = next_state;
    
    if (current_state >= 0 && current_state < STATE_COUNT) {
        state_handlers[current_state]();
    }
}

int main(void) {
    printf("State Machine Demo Started\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == EVENT_COUNT) {
            printf("Invalid event input\n");
            continue;
        }
        
        process_event(event);
        
        if (current_state == STATE_COMPLETE || current_state == STATE_ERROR) {
            printf("Final state reached. Exiting.\n");
            break;
        }
    }
    
    return 0;
}