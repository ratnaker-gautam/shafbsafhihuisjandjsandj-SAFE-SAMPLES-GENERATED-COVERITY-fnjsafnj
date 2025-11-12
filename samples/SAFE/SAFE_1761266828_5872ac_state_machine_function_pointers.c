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

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Enter data value (0-100): ");
    int input;
    if (scanf("%d", &input) == 1) {
        if (input >= 0 && input <= 100) {
            data_value = input;
            printf("Data accepted: %d\n", data_value);
        } else {
            printf("Invalid data range\n");
            current_state = STATE_ERROR;
        }
    } else {
        printf("Invalid input\n");
        current_state = STATE_ERROR;
    }
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (data_value > 0) {
        int result = data_value * 2;
        if (result > 200) {
            current_state = STATE_ERROR;
        } else {
            printf("Processing result: %d\n", result);
            process_count++;
        }
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Processed %d times successfully\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int process_event(Event event) {
    State next_state = current_state;
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                next_state = STATE_READY;
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                next_state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                next_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                next_state = STATE_ERROR;
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                process_count = 0;
                data_value = 0;
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                process_count = 0;
                data_value = 0;
            }
            break;
        default:
            break;
    }
    
    if (next_state != current_state) {
        current_state = next_state;
        return 1;
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
        process_event(event);
    }
    
    printf("Program terminated\n");
    return 0;
}