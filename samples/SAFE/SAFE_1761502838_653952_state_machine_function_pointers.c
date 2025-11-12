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

State current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

void handle_idle(void) {
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
        process_count = 0;
        data_value = 0;
        printf("System reset.\n");
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    } else {
        printf("Invalid command.\n");
    }
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Enter data value (0-100): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        current_state = STATE_ERROR;
        return;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input.\n");
        return;
    }
    
    if (value < 0 || value > 100) {
        printf("Value out of range.\n");
        return;
    }
    
    data_value = (int)value;
    current_state = STATE_PROCESSING;
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", data_value);
    
    if (process_count >= 5) {
        printf("Too many processing attempts.\n");
        current_state = STATE_ERROR;
        return;
    }
    
    process_count++;
    
    if (data_value > 0 && data_value < 50) {
        printf("Processing successful.\n");
        current_state = STATE_COMPLETE;
    } else if (data_value >= 50 && data_value <= 100) {
        printf("Processing failed, retrying...\n");
    } else {
        current_state = STATE_ERROR;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Processing completed successfully.\n");
    printf("Total processing attempts: %d\n", process_count);
    printf("Final data value: %d\n", data_value);
    printf("Enter 'reset' to continue or 'quit' to exit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        current_state = STATE_ERROR;
        return;
    }
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "reset") == 0) {
        current_state = STATE_IDLE;
        process_count = 0;
        data_value = 0;
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    } else {
        printf("Invalid command.\n");
    }
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("An error occurred during processing.\n");
    printf("Enter 'reset' to restart or 'quit' to exit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        exit(1);
    }
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "reset") == 0) {
        current_state = STATE_IDLE;
        process_count = 0;
        data_value = 0;
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    } else {
        printf("Invalid command.\n");
    }
}

int main(void) {
    printf("State Machine Demo Started\n");
    printf("Commands: start, reset, quit\n");
    
    while (1) {
        if (current_state < 0 || current_state >= STATE_COUNT) {
            printf("Invalid state detected.\n");
            return 1;
        }
        state_handlers[current_state]();
    }
    
    return 0;
}