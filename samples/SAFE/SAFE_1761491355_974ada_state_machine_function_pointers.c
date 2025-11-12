//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} event_t;

typedef void (*state_handler_t)(event_t);

void handle_idle(event_t event);
void handle_ready(event_t event);
void handle_processing(event_t event);
void handle_complete(event_t event);
void handle_error(event_t event);

static state_t current_state = STATE_IDLE;
static int data_value = 0;
static int processed_value = 0;

void handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            current_state = STATE_READY;
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(event_t event) {
    switch (event) {
        case EVENT_DATA:
            data_value = 42;
            printf("Data received: %d\n", data_value);
            current_state = STATE_PROCESSING;
            printf("Transition: READY -> PROCESSING\n");
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(event_t event) {
    switch (event) {
        case EVENT_PROCESS:
            if (data_value > 0) {
                processed_value = data_value * 2;
                printf("Processing complete: %d -> %d\n", data_value, processed_value);
                current_state = STATE_COMPLETE;
                printf("Transition: PROCESSING -> COMPLETE\n");
            } else {
                current_state = STATE_ERROR;
                printf("Transition: PROCESSING -> ERROR\n");
            }
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_complete(event_t event) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Operation completed successfully. Result: %d\n", processed_value);
            break;
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(event_t event) {
    switch (event) {
        case EVENT_FAILURE:
            printf("Operation failed\n");
            break;
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

static state_handler_t state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void process_event(event_t event) {
    if (current_state >= STATE_IDLE && current_state <= STATE_ERROR) {
        state_handlers[current_state](event);
    } else {
        printf("Invalid state\n");
    }
}

int main() {
    char input[32];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=EXIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter event (0-6): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 6) {
            break;
        }
        
        if (choice >= 0 && choice <= 5) {
            process_event((event_t)choice);
        } else {
            printf("Invalid event\n");
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}