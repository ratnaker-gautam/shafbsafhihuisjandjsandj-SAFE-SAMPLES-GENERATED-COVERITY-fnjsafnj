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
    printf("Options: 1-Start, 2-Exit\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return;
    }
    if (choice == 1) {
        current_state = STATE_READY;
    } else if (choice == 2) {
        exit(0);
    }
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Enter data value (0-100): ");
    if (scanf("%d", &data_value) != 1) {
        while (getchar() != '\n');
        printf("Invalid input\n");
        current_state = STATE_ERROR;
        return;
    }
    if (data_value < 0 || data_value > 100) {
        printf("Data out of range\n");
        current_state = STATE_ERROR;
        return;
    }
    current_state = STATE_PROCESSING;
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", data_value);
    process_count++;
    if (process_count > 3) {
        printf("Too many processing attempts\n");
        current_state = STATE_ERROR;
        return;
    }
    int result = data_value * 2;
    if (result > 150) {
        printf("Processing failed: result too large\n");
        current_state = STATE_ERROR;
    } else {
        printf("Processing successful: %d\n", result);
        current_state = STATE_COMPLETE;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Options: 1-Reset, 2-Exit\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return;
    }
    if (choice == 1) {
        current_state = STATE_IDLE;
        data_value = 0;
        process_count = 0;
    } else if (choice == 2) {
        exit(0);
    }
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Options: 1-Reset, 2-Exit\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return;
    }
    if (choice == 1) {
        current_state = STATE_IDLE;
        data_value = 0;
        process_count = 0;
    } else if (choice == 2) {
        exit(0);
    }
}

int main(void) {
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        if (current_state >= 0 && current_state < STATE_COUNT) {
            state_handlers[current_state]();
        } else {
            current_state = STATE_ERROR;
        }
        printf("\n");
    }
    
    return 0;
}