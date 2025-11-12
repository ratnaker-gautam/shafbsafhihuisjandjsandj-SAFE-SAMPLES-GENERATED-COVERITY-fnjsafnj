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

void state_idle_handler(void) {
    printf("State: IDLE\n");
    printf("Enter command (start/reset/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "start") == 0) {
        current_state = STATE_READY;
    } else if (strcmp(input, "reset") == 0) {
        current_state = STATE_IDLE;
        data_value = 0;
        process_count = 0;
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    }
}

void state_ready_handler(void) {
    printf("State: READY\n");
    printf("Enter data value (0-100): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        current_state = STATE_ERROR;
        return;
    }
    
    int value;
    if (sscanf(input, "%d", &value) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    if (value < 0 || value > 100) {
        printf("Value out of range\n");
        return;
    }
    
    data_value = value;
    current_state = STATE_PROCESSING;
}

void state_processing_handler(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", data_value);
    
    if (process_count >= 3) {
        current_state = STATE_ERROR;
        return;
    }
    
    if (data_value > 75) {
        current_state = STATE_COMPLETE;
    } else if (data_value < 25) {
        current_state = STATE_ERROR;
    } else {
        data_value += 10;
        process_count++;
        printf("Data incremented, new value: %d\n", data_value);
    }
}

void state_complete_handler(void) {
    printf("State: COMPLETE\n");
    printf("Processing successful! Final value: %d\n", data_value);
    printf("Enter 'reset' to continue or 'quit' to exit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "reset") == 0) {
        current_state = STATE_IDLE;
        data_value = 0;
        process_count = 0;
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    }
}

void state_error_handler(void) {
    printf("State: ERROR\n");
    printf("An error occurred during processing\n");
    printf("Enter 'reset' to continue or 'quit' to exit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "reset") == 0) {
        current_state = STATE_IDLE;
        data_value = 0;
        process_count = 0;
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    }
}

int main(void) {
    StateHandler state_handlers[STATE_COUNT] = {
        state_idle_handler,
        state_ready_handler,
        state_processing_handler,
        state_complete_handler,
        state_error_handler
    };
    
    printf("State Machine Demo Started\n");
    printf("Available commands: start, reset, quit\n");
    
    while (1) {
        if (current_state >= STATE_IDLE && current_state < STATE_COUNT) {
            state_handlers[current_state]();
        } else {
            current_state = STATE_ERROR;
        }
    }
    
    return 0;
}