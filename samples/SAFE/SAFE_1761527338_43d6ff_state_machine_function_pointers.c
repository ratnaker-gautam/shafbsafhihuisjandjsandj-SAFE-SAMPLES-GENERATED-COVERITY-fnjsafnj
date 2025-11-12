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
    {STATE_READY,    STATE_READY,     STATE_PROCESSING, STATE_READY,     STATE_ERROR,     STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR,     STATE_IDLE},
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
        printf("State: PROCESSING - Invalid data value\n");
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
    printf("State: COMPLETE - Processing finished. Total operations: %d\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
}

Event get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number (0-5): ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    if (input >= 0 && input < EVENT_COUNT) {
        return (Event)input;
    }
    return EVENT_COUNT;
}

void process_event(Event event) {
    if (event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    State new_state = transition_table[current_state][event];
    
    if (event == EVENT_DATA) {
        printf("Enter data value: ");
        if (scanf("%d", &data_value) != 1) {
            while (getchar() != '\n');
            printf("Invalid data input\n");
            return;
        }
    }
    
    if (new_state != current_state) {
        current_state = new_state;
        if (current_state < STATE_COUNT) {
            state_handlers[current_state]();
        }
    } else {
        printf("No state change for this event\n");
    }
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Initial state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == EVENT_COUNT) {
            printf("Invalid event selection\n");
            continue;
        }
        
        process_event(event);
        
        if (current_state == STATE_COMPLETE) {
            printf("\nProcess completed successfully\n");
            break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("\nProcess ended with error\n");
            break;
        }
    }
    
    return 0;
}