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
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Data value: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Iteration: %d\n", process_count);
    if (data_value > 0 && process_count < 100) {
        data_value--;
        process_count++;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing finished successfully\n");
}

void handle_error(void) {
    printf("State: ERROR - An error occurred during processing\n");
}

int get_valid_input(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
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
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return;
    }
    
    current_state = new_state;
    
    if (current_state >= 0 && current_state < STATE_COUNT) {
        state_handlers[current_state]();
    }
}

int main(void) {
    printf("Finite State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    
    while (1) {
        int choice = get_valid_input("Enter event number (0-5) or -1 to exit: ", -1, 5);
        
        if (choice == -1) {
            break;
        }
        
        Event event = (Event)choice;
        
        switch (event) {
            case EVENT_START:
                process_count = 0;
                break;
            case EVENT_DATA:
                data_value = get_valid_input("Enter data value (1-100): ", 1, 100);
                break;
            case EVENT_PROCESS:
                break;
            case EVENT_SUCCESS:
                break;
            case EVENT_FAILURE:
                break;
            case EVENT_RESET:
                data_value = 0;
                process_count = 0;
                break;
            default:
                break;
        }
        
        process_event(event);
        printf("\n");
    }
    
    printf("Exiting state machine demo.\n");
    return 0;
}