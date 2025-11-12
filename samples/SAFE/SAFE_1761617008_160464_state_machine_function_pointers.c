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
State current_state = STATE_IDLE;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Enter data values (max 10, -1 to finish):\n");
    data_count = 0;
    int value;
    while (data_count < 10) {
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (value == -1) break;
        data_buffer[data_count++] = value;
    }
    printf("Collected %zu data points\n", data_count);
}

void handle_processing(void) {
    printf("State: PROCESSING - Calculating statistics\n");
    if (data_count == 0) {
        current_state = STATE_ERROR;
        return;
    }
    
    int sum = 0;
    int min = data_buffer[0];
    int max = data_buffer[0];
    
    for (size_t i = 0; i < data_count; i++) {
        sum += data_buffer[i];
        if (data_buffer[i] < min) min = data_buffer[i];
        if (data_buffer[i] > max) max = data_buffer[i];
    }
    
    double average = (double)sum / data_count;
    printf("Results - Min: %d, Max: %d, Average: %.2f\n", min, max, average);
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing finished successfully\n");
}

void handle_error(void) {
    printf("State: ERROR - An error occurred during processing\n");
}

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,     STATE_IDLE, STATE_IDLE,      STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,     STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR,  STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE,  STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,     STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) return;
    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) return;
    current_state = new_state;
    state_handlers[current_state]();
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    printf("Finite State Machine Demo\n");
    
    int running = 1;
    while (running) {
        state_handlers[current_state]();
        print_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event\n");
            continue;
        }
        
        process_event((Event)choice);
    }
    
    printf("Program terminated\n");
    return 0;
}