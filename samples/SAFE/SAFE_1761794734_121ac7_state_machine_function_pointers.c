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
    printf("System in IDLE state. Waiting for start command.\n");
}

void state_ready_handler(void) {
    printf("System READY. Enter data value (0-100): ");
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input. Returning to IDLE.\n");
        current_state = STATE_IDLE;
        while (getchar() != '\n');
        return;
    }
    if (input < 0 || input > 100) {
        printf("Data out of range (0-100). Returning to IDLE.\n");
        current_state = STATE_IDLE;
        return;
    }
    data_value = input;
    current_state = STATE_PROCESSING;
}

void state_processing_handler(void) {
    printf("PROCESSING data: %d\n", data_value);
    if (data_value > 80) {
        printf("Data value too high. Moving to ERROR state.\n");
        current_state = STATE_ERROR;
        return;
    }
    int result = data_value * 2;
    if (result > 150) {
        printf("Processing result too large. Moving to ERROR state.\n");
        current_state = STATE_ERROR;
        return;
    }
    printf("Processing complete. Result: %d\n", result);
    process_count++;
    current_state = STATE_COMPLETE;
}

void state_complete_handler(void) {
    printf("Operation COMPLETE. Total processes: %d\n", process_count);
    printf("Press 1 to reset, 2 to exit: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Resetting.\n");
        current_state = STATE_IDLE;
        while (getchar() != '\n');
        return;
    }
    if (choice == 1) {
        current_state = STATE_IDLE;
    } else if (choice == 2) {
        exit(0);
    } else {
        printf("Invalid choice. Resetting.\n");
        current_state = STATE_IDLE;
    }
}

void state_error_handler(void) {
    printf("ERROR state. Operation failed.\n");
    printf("Press 1 to reset, 2 to exit: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Resetting.\n");
        current_state = STATE_IDLE;
        while (getchar() != '\n');
        return;
    }
    if (choice == 1) {
        current_state = STATE_IDLE;
    } else if (choice == 2) {
        exit(0);
    } else {
        printf("Invalid choice. Resetting.\n");
        current_state = STATE_IDLE;
    }
}

static StateHandler state_handlers[STATE_COUNT] = {
    state_idle_handler,
    state_ready_handler,
    state_processing_handler,
    state_complete_handler,
    state_error_handler
};

int main(void) {
    printf("State Machine System Started\n");
    printf("Available commands: 0=Start, 1=Exit\n");
    
    while (1) {
        if (current_state >= STATE_COUNT || current_state < 0) {
            current_state = STATE_IDLE;
        }
        
        state_handlers[current_state]();
        
        if (current_state == STATE_IDLE) {
            printf("Enter command (0=Start, 1=Exit): ");
            int cmd;
            if (scanf("%d", &cmd) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            if (cmd == 0) {
                current_state = STATE_READY;
            } else if (cmd == 1) {
                break;
            } else {
                printf("Invalid command.\n");
            }
        }
    }
    
    printf("System shutdown. Total processes completed: %d\n", process_count);
    return 0;
}