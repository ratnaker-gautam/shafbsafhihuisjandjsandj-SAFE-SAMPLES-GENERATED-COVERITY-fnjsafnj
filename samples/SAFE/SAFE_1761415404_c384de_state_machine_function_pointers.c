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
    printf("State: READY - Ready to receive data\n");
}

void handle_processing(void) {
    if (data_value < 0) {
        printf("State: PROCESSING - Invalid data detected\n");
        current_state = STATE_ERROR;
        return;
    }
    
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    process_count++;
    
    if (process_count > 100) {
        printf("State: PROCESSING - Process count exceeded\n");
        current_state = STATE_ERROR;
        return;
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing finished successfully\n");
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

int process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                current_state = STATE_READY;
                data_value = 0;
                process_count = 0;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                if (data >= 0 && data <= 1000) {
                    data_value = data;
                    current_state = STATE_PROCESSING;
                } else {
                    current_state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                current_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                current_state = STATE_ERROR;
            } else if (event == EVENT_RESET) {
                current_state = STATE_IDLE;
                data_value = 0;
                process_count = 0;
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
            return -1;
    }
    
    return 0;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA (requires value 0-1000)\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    int running = 1;
    int choice;
    int data_input;
    
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
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event\n");
            continue;
        }
        
        if (choice == EVENT_DATA) {
            printf("Enter data value (0-1000): ");
            if (scanf("%d", &data_input) != 1) {
                while (getchar() != '\n');
                printf("Invalid data input\n");
                continue;
            }
        } else {
            data_input = 0;
        }
        
        if (process_event((Event)choice, data_input) != 0) {
            printf("Event processing failed\n");
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}