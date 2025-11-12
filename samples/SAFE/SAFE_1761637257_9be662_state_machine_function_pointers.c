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
    {STATE_READY,    STATE_IDLE,      STATE_IDLE,      STATE_IDLE,      STATE_IDLE,      STATE_IDLE},
    {STATE_READY,    STATE_READY,     STATE_PROCESSING,STATE_READY,     STATE_ERROR,     STATE_IDLE},
    {STATE_PROCESSING,STATE_PROCESSING,STATE_PROCESSING,STATE_COMPLETE, STATE_ERROR,     STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR,     STATE_ERROR,     STATE_ERROR,     STATE_ERROR,     STATE_IDLE}
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
    if (data_value < 0) {
        printf("State: PROCESSING - Invalid data detected\n");
        return;
    }
    
    int result = data_value * 2;
    if (result < data_value) {
        printf("State: PROCESSING - Integer overflow detected\n");
        return;
    }
    
    printf("State: PROCESSING - Processing data: %d -> %d\n", data_value, result);
    process_count++;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing completed. Total processes: %d\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
}

int get_valid_event(void) {
    char input[32];
    int event = -1;
    
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &event) != 1) {
        return -1;
    }
    
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    return event;
}

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    State next_state = transition_table[current_state][event];
    
    if (next_state < 0 || next_state >= STATE_COUNT) {
        return;
    }
    
    if (event == EVENT_DATA) {
        char input[32];
        printf("Enter data value: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &data_value) != 1) {
                data_value = 0;
            }
        }
    }
    
    current_state = next_state;
    
    if (current_state >= 0 && current_state < STATE_COUNT) {
        state_handlers[current_state]();
    }
}

int main(void) {
    printf("Finite State Machine Demo\n");
    printf("=========================\n");
    
    state_handlers[current_state]();
    
    while (1) {
        int event = get_valid_event();
        
        if (event == -1) {
            printf("Invalid event or input error. Try again.\n");
            continue;
        }
        
        process_event((Event)event);
        
        if (current_state == STATE_IDLE && event == EVENT_RESET) {
            data_value = 0;
            process_count = 0;
        }
        
        if (current_state == STATE_COMPLETE) {
            printf("\nProcessing cycle completed. Reset to continue.\n");
        }
    }
    
    return 0;
}