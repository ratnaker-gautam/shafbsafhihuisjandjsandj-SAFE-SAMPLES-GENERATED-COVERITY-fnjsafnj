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
    if (process_count < 0) {
        current_state = STATE_ERROR;
        return;
    }
    
    printf("State: PROCESSING - Processing data: %d (iteration %d)\n", data_value, process_count + 1);
    
    if (data_value > 1000) {
        current_state = STATE_ERROR;
        return;
    }
    
    data_value *= 2;
    process_count++;
    
    if (process_count >= 3) {
        current_state = STATE_COMPLETE;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Final result: %d\n", data_value);
}

void handle_error(void) {
    printf("State: ERROR - Invalid operation detected\n");
}

int get_valid_input(const char* prompt, int min_val, int max_val) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
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
}

int main(void) {
    printf("Finite State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    
    while (1) {
        state_handlers[current_state]();
        
        if (current_state == STATE_COMPLETE || current_state == STATE_ERROR) {
            printf("Final state reached. Type 5 to reset or 6 to exit: ");
        } else {
            printf("Enter event (0-5) or 6 to exit: ");
        }
        
        int choice = get_valid_input("", 0, 6);
        if (choice == 6) {
            break;
        }
        
        if (choice == 0) {
            process_event(EVENT_START);
        } else if (choice == 1) {
            if (current_state == STATE_READY) {
                data_value = get_valid_input("Enter data value (1-100): ", 1, 100);
            }
            process_event(EVENT_DATA);
        } else if (choice == 2) {
            if (current_state == STATE_READY && data_value > 0) {
                process_count = 0;
                process_event(EVENT_PROCESS);
            }
        } else if (choice == 3) {
            process_event(EVENT_SUCCESS);
        } else if (choice == 4) {
            process_event(EVENT_FAILURE);
        } else if (choice == 5) {
            data_value = 0;
            process_count = 0;
            process_event(EVENT_RESET);
        }
    }
    
    printf("Program terminated.\n");
    return 0;
}