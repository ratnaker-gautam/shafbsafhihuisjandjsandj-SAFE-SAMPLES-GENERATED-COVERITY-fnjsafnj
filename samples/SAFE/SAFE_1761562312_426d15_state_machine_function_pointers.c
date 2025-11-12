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
    {STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_IDLE},
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
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (data_value > 0) {
        int result = data_value * 2;
        if (result < data_value) {
            printf("Integer overflow detected\n");
            current_state = STATE_ERROR;
            return;
        }
        data_value = result;
        process_count++;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Final value: %d, Process cycles: %d\n", data_value, process_count);
}

void handle_error(void) {
    printf("State: ERROR - Operation failed\n");
}

Event get_user_event(void) {
    char input[32];
    printf("Enter event (start, data, process, success, failure, reset): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_COUNT;
}

int get_integer_input(const char* prompt, int min, int max) {
    char buffer[32];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    return (int)value;
}

void process_event(Event event) {
    if (event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    if (event == EVENT_DATA) {
        int new_data = get_integer_input("Enter data value (1-1000): ", 1, 1000);
        if (new_data == -1) {
            printf("Invalid data input\n");
            return;
        }
        data_value = new_data;
    }
    
    State next_state = transition_table[current_state][event];
    if (next_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    current_state = next_state;
    
    if (current_state < STATE_COUNT && state_handlers[current_state] != NULL) {
        state_handlers[current_state]();
    }
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Available events: start, data, process, success, failure, reset\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == EVENT_COUNT) {
            printf("Invalid event or input error\n");
            continue;
        }
        
        process_event(event);
        
        if (current_state == STATE_COMPLETE) {
            printf("Operation completed successfully\n");
            break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Operation failed with error\n");
            break;
        }
    }
    
    return 0;
}