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
    EVENT_QUIT,
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
    printf("State: READY - Enter data values (0-100), 'p' to process, 'r' to reset\n");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "p") == 0) {
        if (data_count > 0) {
            printf("Data collected: %zu values\n", data_count);
        } else {
            printf("No data to process\n");
        }
    } else if (strcmp(input, "r") == 0) {
        data_count = 0;
        processing_result = 0;
        printf("System reset\n");
    } else {
        char *endptr;
        long value = strtol(input, &endptr, 10);
        if (endptr != input && *endptr == '\0' && value >= 0 && value <= 100) {
            if (data_count < sizeof(data_buffer)/sizeof(data_buffer[0])) {
                data_buffer[data_count++] = (int)value;
                printf("Added value: %d (total: %zu)\n", (int)value, data_count);
            } else {
                printf("Buffer full\n");
            }
        } else {
            printf("Invalid input. Enter number 0-100, 'p', or 'r'\n");
        }
    }
}

void handle_processing(void) {
    printf("State: PROCESSING - Calculating average...\n");
    
    if (data_count == 0) {
        printf("No data to process\n");
        return;
    }
    
    long long sum = 0;
    for (size_t i = 0; i < data_count; i++) {
        sum += data_buffer[i];
    }
    
    if (data_count > 0) {
        processing_result = (int)(sum / data_count);
        printf("Average calculated: %d\n", processing_result);
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Result: %d. Press 'r' to reset\n", processing_result);
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "r") == 0) {
        data_count = 0;
        processing_result = 0;
        printf("System reset\n");
    } else {
        printf("Invalid input. Press 'r' to reset\n");
    }
}

void handle_error(void) {
    printf("State: ERROR - System error occurred. Press 'r' to reset\n");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "r") == 0) {
        data_count = 0;
        processing_result = 0;
        printf("System reset\n");
    } else {
        printf("Invalid input. Press 'r' to reset\n");
    }
}

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,      STATE_IDLE,   STATE_ERROR, STATE_IDLE, STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE, STATE_READY},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE, STATE_PROCESSING},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE, STATE_COMPLETE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR,    STATE_ERROR,  STATE_ERROR, STATE_IDLE, STATE_ERROR}
};

int main(void) {
    State current_state = STATE_IDLE;
    
    printf("State Machine Demo - Data Processing System\n");
    printf("Commands: start, data, process, success, failure, reset, quit\n");
    
    while (1) {
        state_handlers[current_state]();
        
        printf("Enter event: ");
        char input[32];
        if (fgets(input, sizeof(input), stdin