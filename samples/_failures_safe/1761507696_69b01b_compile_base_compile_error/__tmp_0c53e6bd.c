//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_EXIT,
    EVENT_COUNT
};

typedef void (*StateHandler)(enum Event);

void handle_idle(enum Event event);
void handle_ready(enum Event event);
void handle_processing(enum Event event);
void handle_complete(enum Event event);
void handle_error(enum Event event);

StateHandler state_table[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

enum State current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

void transition_to(enum State new_state) {
    if (new_state >= 0 && new_state < STATE_COUNT) {
        current_state = new_state;
    }
}

void handle_idle(enum Event event) {
    switch (event) {
        case EVENT_START:
            printf("System starting...\n");
            transition_to(STATE_READY);
            break;
        case EVENT_RESET:
            printf("Already in idle state.\n");
            break;
        default:
            printf("Invalid event for idle state.\n");
            break;
    }
}

void handle_ready(enum Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            if (data_value >= 0 && data_value <= 100) {
                printf("Data ready: %d\n", data_value);
                transition_to(STATE_PROCESSING);
            } else {
                printf("Invalid data value.\n");
                transition_to(STATE_ERROR);
            }
            break;
        case EVENT_RESET:
            printf("Resetting to idle.\n");
            transition_to(STATE_IDLE);
            break;
        default:
            printf("Invalid event for ready state.\n");
            break;
    }
}

void handle_processing(enum Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            if (process_count < 10) {
                process_count++;
                printf("Processing successful. Count: %d\n", process_count);
                transition_to(STATE_COMPLETE);
            } else {
                printf("Process count exceeded.\n");
                transition_to(STATE_ERROR);
            }
            break;
        case EVENT_FAILURE:
            printf("Processing failed.\n");
            transition_to(STATE_ERROR);
            break;
        case EVENT_RESET:
            printf("Resetting to idle.\n");
            process_count = 0;
            transition_to(STATE_IDLE);
            break;
        default:
            printf("Invalid event for processing state.\n");
            break;
    }
}

void handle_complete(enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting to idle.\n");
            process_count = 0;
            transition_to(STATE_IDLE);
            break;
        case EVENT_START:
            printf("Starting new cycle.\n");
            transition_to(STATE_READY);
            break;
        default:
            printf("Invalid event for complete state.\n");
            break;
    }
}

void handle_error(enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            process_count = 0;
            data_value = 0;
            transition_to(STATE_IDLE);
            break;
        default:
            printf("Invalid event for error state.\n");
            break;
    }
}

void process_event(enum Event event) {
    if (current_state >= 0 && current_state < STATE_COUNT) {
        state_table[current_state](event);
    }
}

enum Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA_READY, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET, 6: EXIT\n");
    printf("Current state: ");
    switch (current_state) {
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
        return EVENT_COUNT;
    }
    
    if (input >= 0 && input < EVENT_COUNT) {
        return (enum Event)input;
    }
    return EVENT_COUNT;
}

int main() {
    printf("State Machine Demo Started\n");
    
    while (1) {
        enum Event event = get_user_event();
        
        if (event == EVENT_COUNT) {
            printf("Invalid event.\n");
            continue;
        }
        
        if (event == EVENT_EXIT) {
            printf("Exiting...\n");
            break;
        }
        
        if (event == EVENT_DATA_READY) {
            printf("Enter data value (0-100): ");
            if (scanf("%d", &data_value) != 1) {
                while (getchar() !=