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
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Enter data value (0-100): ");
    int result = scanf("%d", &data_value);
    if (result != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        current_state = STATE_ERROR;
        return;
    }
    if (data_value < 0 || data_value > 100) {
        printf("Data value out of range\n");
        current_state = STATE_ERROR;
        return;
    }
    printf("Data accepted: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (process_count >= 3) {
        printf("Maximum processing attempts reached\n");
        current_state = STATE_ERROR;
        return;
    }
    process_count++;
    int processed_value = data_value * 2;
    if (processed_value < 0) {
        printf("Integer overflow detected\n");
        current_state = STATE_ERROR;
        return;
    }
    printf("Processed value: %d\n", processed_value);
}

void handle_complete(void) {
    printf("State: COMPLETE - Operation successful\n");
    printf("Final data: %d, Processing attempts: %d\n", data_value, process_count);
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
}

int process_event(Event event) {
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                current_state = STATE_READY;
                return 1;
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                current_state = STATE_PROCESSING;
                return 1;
            } else if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                return 1;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                current_state = STATE_COMPLETE;
                return 1;
            } else if (event == EVENT_FAILURE) {
                current_state = STATE_ERROR;
                return 1;
            } else if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                process_count = 0;
                return 1;
            }
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                process_count = 0;
                return 1;
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                process_count = 0;
                return 1;
            }
            break;
        default:
            break;
    }
    return 0;
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
    printf("State Machine Demo\n");
    printf("==================\n");
    
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
        
        if (choice < 0 || choice > 6) {
            printf("Invalid choice\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        Event event = (Event)choice;
        if (!process_event(event)) {
            printf("Event not valid in current state\n");
        }
    }
    
    printf("Program terminated\n");
    return 0;
}