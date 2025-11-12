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
    printf("State: READY - Data loaded: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Working on data: %d\n", data_value);
    process_count++;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processed %d times\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - Something went wrong\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

typedef struct {
    State next_state;
    int valid;
} Transition;

Transition state_table[STATE_COUNT][EVENT_COUNT] = {
    {{STATE_READY, 1}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 1}},
    {{STATE_READY, 0}, {STATE_READY, 1}, {STATE_PROCESSING, 1}, {STATE_READY, 0}, {STATE_ERROR, 1}, {STATE_IDLE, 1}},
    {{STATE_PROCESSING, 0}, {STATE_PROCESSING, 0}, {STATE_PROCESSING, 0}, {STATE_COMPLETE, 1}, {STATE_ERROR, 1}, {STATE_IDLE, 1}},
    {{STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_IDLE, 1}},
    {{STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_IDLE, 1}}
};

void process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    Transition trans = state_table[current_state][event];
    if (!trans.valid) {
        printf("Invalid transition from current state\n");
        return;
    }
    
    if (event == EVENT_DATA) {
        if (data < 0 || data > 100) {
            printf("Invalid data range\n");
            return;
        }
        data_value = data;
    }
    
    current_state = trans.next_state;
    state_handlers[current_state]();
}

int main(void) {
    char input[32];
    int choice, value;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT\n");
    
    while (1) {
        printf("\nEnter command: ");
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
            printf("Invalid command\n");
            continue;
        }
        
        if (choice == 1) {
            printf("Enter data value (0-100): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                continue;
            }
            if (sscanf(input, "%d", &value) != 1) {
                printf("Invalid data\n");
                continue;
            }
            process_event((Event)choice, value);
        } else {
            process_event((Event)choice, 0);
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}