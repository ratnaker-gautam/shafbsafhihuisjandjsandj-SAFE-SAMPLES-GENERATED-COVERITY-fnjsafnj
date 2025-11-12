//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

enum event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
};

typedef void (*state_handler)(enum event);

void idle_handler(enum event ev);
void ready_handler(enum event ev);
void processing_handler(enum event ev);
void complete_handler(enum event ev);
void error_handler(enum event ev);

static state_handler handlers[STATE_COUNT] = {
    idle_handler,
    ready_handler,
    processing_handler,
    complete_handler,
    error_handler
};

static enum state current_state = STATE_IDLE;
static int data_value = 0;
static int process_count = 0;

void idle_handler(enum event ev) {
    switch (ev) {
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

void ready_handler(enum event ev) {
    switch (ev) {
        case EVENT_DATA_READY:
            if (data_value < 100) {
                data_value += 10;
                printf("Data prepared: %d\n", data_value);
                current_state = STATE_PROCESSING;
                printf("Transition: READY -> PROCESSING\n");
            } else {
                printf("Data limit reached, transitioning to ERROR\n");
                current_state = STATE_ERROR;
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

void processing_handler(enum event ev) {
    switch (ev) {
        case EVENT_SUCCESS:
            process_count++;
            printf("Processing successful. Total processes: %d\n", process_count);
            current_state = STATE_COMPLETE;
            printf("Transition: PROCESSING -> COMPLETE\n");
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

void complete_handler(enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        case EVENT_START:
            printf("Transition: COMPLETE -> READY\n");
            current_state = STATE_READY;
            data_value = 0;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void error_handler(enum event ev) {
    switch (ev) {
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

void process_event(enum event ev) {
    if (ev >= EVENT_COUNT || current_state >= STATE_COUNT) {
        printf("Invalid event or state\n");
        return;
    }
    handlers[current_state](ev);
}

int main() {
    char input[32];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Events: 0=START, 1=DATA_READY, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=EXIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf(" | Enter event number: ");
        
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
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event number\n");
            continue;
        }
        
        process_event((enum event)choice);
    }
    
    printf("Exiting state machine demo\n");
    return 0;
}