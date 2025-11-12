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

void handle_idle(enum Event event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            current_state = STATE_READY;
            data_value = 0;
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(enum Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            if (data_value >= 0 && data_value < 100) {
                data_value += 10;
                printf("Data prepared: %d\n", data_value);
                current_state = STATE_PROCESSING;
                printf("Transition: READY -> PROCESSING\n");
            } else {
                current_state = STATE_ERROR;
                printf("Transition: READY -> ERROR\n");
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(enum Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            if (process_count < 5) {
                process_count++;
                printf("Processing successful (%d/5)\n", process_count);
                if (process_count >= 5) {
                    current_state = STATE_COMPLETE;
                    printf("Transition: PROCESSING -> COMPLETE\n");
                } else {
                    current_state = STATE_READY;
                    printf("Transition: PROCESSING -> READY\n");
                }
            }
            break;
        case EVENT_FAILURE:
            printf("Processing failed\n");
            current_state = STATE_ERROR;
            printf("Transition: PROCESSING -> ERROR\n");
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_complete(enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(enum Event event) {
    if (current_state >= 0 && current_state < STATE_COUNT) {
        state_table[current_state](event);
    }
}

enum Event get_user_event() {
    char input[32];
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
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr != input && choice >= 0 && choice <= 6) {
            if (choice == 6) return EVENT_COUNT;
            return (enum Event)choice;
        }
    }
    printf("Invalid input\n");
    return EVENT_COUNT;
}

int main() {
    printf("State Machine Demo Started\n");
    
    while (1) {
        enum Event event = get_user_event();
        if (event == EVENT_COUNT) break;
        
        if (event >= 0 && event < EVENT_COUNT) {
            process_event(event);
        } else {