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
static state_handler_t handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

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
            current_state = STATE_ERROR;
            break;
    }
}

void handle_ready(event_t event) {
    switch (event) {
        case EVENT_DATA:
            printf("Transition: READY -> PROCESSING\n");
            current_state = STATE_PROCESSING;
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
    switch (event) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> COMPLETE\n");
            current_state = STATE_COMPLETE;
            break;
        case EVENT_FAILURE:
            printf("Transition: PROCESSING -> ERROR\n");
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
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void handle_error(event_t event) {
    switch (event) {
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
    if (current_state >= STATE_IDLE && current_state <= STATE_ERROR) {
        handlers[current_state](event);
    } else {
        printf("Invalid current state\n");
        current_state = STATE_ERROR;
    }
}

event_t get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number (0-5): ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_FAILURE;
    }
    
    if (input >= 0 && input <= 5) {
        return (event_t)input;
    } else {
        printf("Invalid event number\n");
        return EVENT_FAILURE;
    }
}

int main() {
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    int running = 1;
    while (running) {
        event_t event = get_user_event();
        
        if (event == EVENT_FAILURE && current_state != STATE_PROCESSING) {
            printf("Invalid input, please try again\n");
            continue;
        }
        
        process_event(event);
        
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE\n"); break;
            case STATE_READY: printf("READY\n"); break;
            case STATE_PROCESSING: printf("PROCESSING\n"); break;
            case STATE_COMPLETE: printf("COMPLETE\n"); break;
            case STATE_ERROR: printf("ERROR\n"); break;
            default: printf("UNKNOWN\n"); break;
        }
        
        if (current_state == STATE_COMPLETE) {
            printf("Process completed successfully!\n");
            running = 0;
        }
    }
    
    return 0;
}