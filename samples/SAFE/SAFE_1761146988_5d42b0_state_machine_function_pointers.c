//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAIL,
    EVENT_RESET
} Event;

typedef void (*StateHandler)(Event);

State current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

void handle_idle(Event event) {
    switch (event) {
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

void handle_ready(Event event) {
    switch (event) {
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

void handle_processing(Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            process_count++;
            printf("Processing successful. Count: %d\n", process_count);
            if (process_count >= 3) {
                printf("Transition: PROCESSING -> DONE\n");
                current_state = STATE_DONE;
            } else {
                printf("Transition: PROCESSING -> READY\n");
                current_state = STATE_READY;
            }
            break;
        case EVENT_FAIL:
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

void handle_done(Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: DONE -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for DONE state\n");
            current_state = STATE_ERROR;
            break;
    }
}

void handle_error(Event event) {
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

StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAIL, 5: RESET\n");
    printf("Enter event number: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_FAIL;
    }
    
    if (input >= 0 && input <= 5) {
        return (Event)input;
    }
    
    return EVENT_FAIL;
}

int main() {
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        
        if (current_state >= STATE_IDLE && current_state <= STATE_ERROR) {
            state_handlers[current_state](event);
        } else {
            printf("Invalid state detected\n");
            break;
        }
        
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\n");
        
        if (current_state == STATE_DONE) {
            printf("Process completed successfully\n");
            break;
        }
    }
    
    return 0;
}