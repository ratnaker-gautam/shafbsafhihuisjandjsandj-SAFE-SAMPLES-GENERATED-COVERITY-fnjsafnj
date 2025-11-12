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

int data_buffer[10];
size_t data_count = 0;
int processing_result = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Enter data values (0-100, -1 to stop): ");
    int value;
    data_count = 0;
    
    while (data_count < 10) {
        if (scanf("%d", &value) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (value == -1) {
            break;
        }
        
        if (value < 0 || value > 100) {
            printf("Value out of range (0-100). Try again.\n");
            continue;
        }
        
        data_buffer[data_count++] = value;
        printf("Added value: %d (%zu/10)\n", value, data_count);
    }
    
    if (data_count > 0) {
        printf("Data collection complete. %zu values entered.\n", data_count);
    } else {
        printf("No data entered.\n");
    }
}

void handle_processing(void) {
    printf("State: PROCESSING - Calculating average...\n");
    
    if (data_count == 0) {
        printf("Error: No data to process.\n");
        return;
    }
    
    long long sum = 0;
    for (size_t i = 0; i < data_count; i++) {
        sum += data_buffer[i];
    }
    
    if (data_count > 0) {
        processing_result = (int)(sum / data_count);
        printf("Processing complete. Average: %d\n", processing_result);
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Result: %d\n", processing_result);
    printf("Data processed successfully.\n");
}

void handle_error(void) {
    printf("State: ERROR - An error occurred during processing.\n");
}

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,      STATE_IDLE, STATE_IDLE,      STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,      STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR,  STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE,   STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,      STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

State current_state = STATE_IDLE;

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return;
    }
    
    current_state = new_state;
    state_handlers[current_state]();
}

int main(void) {
    printf("State Machine Demo - Data Processing\n");
    printf("Commands: s=start, d=data, p=process, c=success, f=failure, r=reset, q=quit\n");
    
    char input[2];
    
    while (1) {
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        while (getchar() != '\n');
        
        Event event;
        switch (input[0]) {
            case 's': event = EVENT_START; break;
            case 'd': event = EVENT_DATA; break;
            case 'p': event = EVENT_PROCESS; break;
            case 'c': event = EVENT_SUCCESS; break;
            case 'f': event = EVENT_FAILURE; break;
            case 'r': event = EVENT_RESET; break;
            case 'q': 
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid command. Use s,d,p,c,f,r,q\n");
                continue;
        }
        
        process_event(event);
    }
    
    return 0;
}