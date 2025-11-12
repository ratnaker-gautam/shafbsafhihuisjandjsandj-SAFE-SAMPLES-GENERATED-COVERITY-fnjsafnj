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
    printf("State: READY - Ready to receive data\n");
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (data_value > 0) {
        process_count += data_value;
        printf("Processing complete. Total: %d\n", process_count);
    } else {
        printf("Invalid data for processing\n");
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Final result: %d\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
}

static StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

static State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,     STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    current_state = new_state;
    
    if (current_state >= 0 && current_state < STATE_COUNT) {
        state_handlers[current_state]();
    }
}

int get_valid_input(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d\n", min, max);
    }
}

int main(void) {
    printf("State Machine Demo - Data Processor\n");
    printf("Events: 0=Start, 1=Data, 2=Process, 3=Success, 4=Failure, 5=Reset\n");
    
    while (1) {
        printf("\n");
        int choice = get_valid_input("Enter event (0-5, 6 to exit): ", 0, 6);
        
        if (choice == 6) {
            break;
        }
        
        if (choice == EVENT_DATA) {
            data_value = get_valid_input("Enter data value (1-100): ", 1, 100);
        }
        
        process_event((Event)choice);
    }
    
    printf("Program terminated\n");
    return 0;
}