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
    NUM_STATES
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} event_t;

typedef void (*state_handler_t)(event_t);

void handle_idle(event_t event);
void handle_ready(event_t event);
void handle_processing(event_t event);
void handle_complete(event_t event);
void handle_error(event_t event);

static state_handler_t state_table[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

static state_t current_state = STATE_IDLE;
static int data_value = 0;
static int process_count = 0;

void handle_idle(event_t event) {
    switch(event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            current_state = STATE_READY;
            data_value = 0;
            process_count = 0;
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

void handle_ready(event_t event) {
    switch(event) {
        case EVENT_DATA:
            if (data_value < 100) {
                data_value += 10;
                printf("Data updated: %d\n", data_value);
            } else {
                printf("Data limit reached\n");
            }
            break;
        case EVENT_PROCESS:
            if (data_value > 0) {
                printf("Transition: READY -> PROCESSING\n");
                current_state = STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void handle_processing(event_t event) {
    switch(event) {
        case EVENT_SUCCESS:
            process_count++;
            printf("Processing successful. Count: %d\n", process_count);
            if (process_count >= 3) {
                printf("Transition: PROCESSING -> COMPLETE\n");
                current_state = STATE_COMPLETE;
            } else {
                printf("Transition: PROCESSING -> READY\n");
                current_state = STATE_READY;
            }
            break;
        case EVENT_FAILURE:
            printf("Processing failed\n");
            current_state = STATE_ERROR;
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void handle_complete(event_t event) {
    switch(event) {
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
    switch(event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(event_t event) {
    if (event >= 0 && event < NUM_EVENTS && current_state >= 0 && current_state < NUM_STATES) {
        state_table[current_state](event);
    } else {
        printf("Invalid state or event\n");
        current_state = STATE_ERROR;
    }
}

event_t get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Current state: ");
    switch(current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\nEnter event number: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return NUM_EVENTS;
    }
    
    if (input >= 0 && input < NUM_EVENTS) {
        return (event_t)input;
    } else {
        return NUM_EVENTS;
    }
}

int main() {
    printf("State Machine Demo\n");
    printf("==================\n");
    
    int running = 1;
    while (running) {
        event_t event = get_user_event();
        if (event == NUM_EVENTS) {
            printf("Invalid input or exit condition\n");
            running = 0;
        } else {
            process_event(event);
        }
    }
    
    return 0;
}