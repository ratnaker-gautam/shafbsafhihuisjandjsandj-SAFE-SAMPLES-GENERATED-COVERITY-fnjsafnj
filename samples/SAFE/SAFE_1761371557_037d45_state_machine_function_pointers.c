//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

static const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

static const char* event_names[EVENT_COUNT] = {
    "START", "DATA_READY", "PROCESS", "SUCCESS", "FAILURE", "RESET"
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
            printf("Invalid event %s for IDLE state\n", event_names[ev]);
            break;
    }
}

void ready_handler(enum event ev) {
    switch (ev) {
        case EVENT_DATA_READY:
            if (data_value < 100) {
                data_value += 10;
                printf("Data prepared: %d\n", data_value);
            }
            break;
        case EVENT_PROCESS:
            if (data_value > 0) {
                printf("Transition: READY -> PROCESSING\n");
                current_state = STATE_PROCESSING;
            } else {
                printf("No data available for processing\n");
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            break;
        default:
            printf("Invalid event %s for READY state\n", event_names[ev]);
            break;
    }
}

void processing_handler(enum event ev) {
    switch (ev) {
        case EVENT_SUCCESS:
            if (process_count < 5) {
                process_count++;
                printf("Processing successful (%d/5)\n", process_count);
                if (process_count >= 5) {
                    printf("Transition: PROCESSING -> COMPLETE\n");
                    current_state = STATE_COMPLETE;
                }
            }
            break;
        case EVENT_FAILURE:
            printf("Transition: PROCESSING -> ERROR\n");
            current_state = STATE_ERROR;
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
            break;
        default:
            printf("Invalid event %s for PROCESSING state\n", event_names[ev]);
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
        default:
            printf("Invalid event %s for COMPLETE state\n", event_names[ev]);
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
            printf("Invalid event %s for ERROR state\n", event_names[ev]);
            break;
    }
}

void process_event(enum event ev) {
    if (ev >= 0 && ev < EVENT_COUNT && current_state >= 0 && current_state < STATE_COUNT) {
        handlers[current_state](ev);
    }
}

int main(void) {
    char input[32];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("0: START, 1: DATA_READY, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET, 6: EXIT\n");
    
    while (1) {
        printf("\nCurrent state: %s\n", state_names[current_state]);
        printf("Enter event number: ");
        
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
        
        if (choice >= 0 && choice < EVENT_COUNT) {
            process_event((enum event)choice);
        } else {
            printf("Invalid event number\n");
        }
    }
    
    return 0;
}