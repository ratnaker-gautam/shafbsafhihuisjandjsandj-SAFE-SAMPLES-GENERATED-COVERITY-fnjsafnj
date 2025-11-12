//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

typedef void (*StateHandler)(enum Event);

void idle_handler(enum Event event);
void ready_handler(enum Event event);
void processing_handler(enum Event event);
void done_handler(enum Event event);
void error_handler(enum Event event);

static enum State current_state = STATE_IDLE;
static int data_value = 0;
static int process_count = 0;

void idle_handler(enum Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transitioning from IDLE to READY\n");
            current_state = STATE_READY;
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void ready_handler(enum Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            if (data_value >= 0 && data_value <= 100) {
                printf("Data value %d accepted, transitioning to PROCESSING\n", data_value);
                current_state = STATE_PROCESSING;
            } else {
                printf("Invalid data value, transitioning to ERROR\n");
                current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from READY to IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            break;
        default:
            printf("Invalid event for READY state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void processing_handler(enum Event event) {
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (process_count < 10) {
                int result = data_value * 2;
                if (result < 0 || result > 1000) {
                    printf("Processing result out of bounds, transitioning to ERROR\n");
                    current_state = STATE_ERROR;
                } else {
                    printf("Processing complete, result: %d, transitioning to DONE\n", result);
                    current_state = STATE_DONE;
                    process_count++;
                }
            } else {
                printf("Process count limit reached, transitioning to ERROR\n");
                current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from PROCESSING to IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void done_handler(enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from DONE to IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            break;
        case EVENT_START:
            printf("Transitioning from DONE to READY\n");
            current_state = STATE_READY;
            break;
        default:
            printf("Invalid event for DONE state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void error_handler(enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from ERROR to IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        default:
            printf("Only RESET allowed in ERROR state\n");
            break;
    }
}

StateHandler state_handlers[] = {
    idle_handler,
    ready_handler,
    processing_handler,
    done_handler,
    error_handler
};

void process_event(enum Event event) {
    if (current_state >= STATE_IDLE && current_state <= STATE_ERROR) {
        state_handlers[current_state](event);
    } else {
        printf("Invalid state detected\n");
        current_state = STATE_ERROR;
    }
}

int main(void) {
    char input[32];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=ERROR, 4=RESET, 5=EXIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Data: %d | Process count: %d\n", data_value, process_count);
        
        printf("Enter command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 5) {
            break;
        }
        
        if (choice < 0 || choice > 4) {
            printf("Invalid command\n");
            continue;
        }
        
        if (choice == 1 && current_state ==