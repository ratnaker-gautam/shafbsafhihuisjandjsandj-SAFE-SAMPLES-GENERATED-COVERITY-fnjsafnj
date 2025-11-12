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
    printf("State: IDLE - Waiting for start event\n");
}

void handle_ready(void) {
    printf("State: READY - Data received: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (data_value > 0) {
        int result = data_value * 2;
        if (result < data_value) {
            current_state = STATE_ERROR;
            return;
        }
        data_value = result;
        process_count++;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Final value: %d, Processes: %d\n", data_value, process_count);
}

void handle_error(void) {
    printf("State: ERROR - Invalid operation detected\n");
}

int validate_event(int event) {
    return event >= 0 && event < EVENT_COUNT;
}

void transition(Event event, int data) {
    if (!validate_event(event)) {
        return;
    }

    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                current_state = STATE_READY;
                data_value = data;
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                if (data >= 0 && data <= 1000) {
                    data_value = data;
                } else {
                    current_state = STATE_ERROR;
                }
            } else if (event == EVENT_PROCESS) {
                current_state = STATE_PROCESSING;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                current_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                current_state = STATE_ERROR;
            }
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                process_count = 0;
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                process_count = 0;
            }
            break;
        default:
            break;
    }
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START (with data 0-1000)\n");
    printf("1 - DATA (new value 0-1000)\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Current state: ");
}

int main(void) {
    int running = 1;
    int choice;
    int input_data;

    printf("State Machine Demo\n");

    while (running) {
        state_handlers[current_state]();
        print_menu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }

        if (choice == 6) {
            running = 0;
            continue;
        }

        if (choice == 0 || choice == 1) {
            printf("Enter data value (0-1000): ");
            if (scanf("%d", &input_data) != 1) {
                while (getchar() != '\n');
                printf("Invalid data\n");
                continue;
            }
        } else {
            input_data = 0;
        }

        transition(choice, input_data);
    }

    printf("Exiting state machine\n");
    return 0;
}