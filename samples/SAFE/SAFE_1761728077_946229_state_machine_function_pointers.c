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

void state_idle_handler(void) {
    printf("State: IDLE\n");
}

void state_ready_handler(void) {
    printf("State: READY\n");
}

void state_processing_handler(void) {
    printf("State: PROCESSING - Value: %d\n", data_value);
    if (data_value > 0) {
        data_value *= 2;
        process_count++;
    }
}

void state_complete_handler(void) {
    printf("State: COMPLETE - Final value: %d, Processes: %d\n", data_value, process_count);
}

void state_error_handler(void) {
    printf("State: ERROR\n");
}

static StateHandler state_handlers[STATE_COUNT] = {
    state_idle_handler,
    state_ready_handler,
    state_processing_handler,
    state_complete_handler,
    state_error_handler
};

static State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,      STATE_IDLE,    STATE_ERROR, STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY,  STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR,    STATE_ERROR,    STATE_ERROR, STATE_IDLE}
};

int validate_event(int event) {
    return event >= 0 && event < EVENT_COUNT;
}

int validate_state(int state) {
    return state >= 0 && state < STATE_COUNT;
}

void process_event(Event event) {
    if (!validate_event(event) || !validate_state(current_state)) {
        return;
    }
    
    State new_state = transition_table[current_state][event];
    if (!validate_state(new_state)) {
        return;
    }
    
    current_state = new_state;
    
    if (current_state < STATE_COUNT && state_handlers[current_state] != NULL) {
        state_handlers[current_state]();
    }
}

void reset_machine(void) {
    current_state = STATE_IDLE;
    data_value = 0;
    process_count = 0;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=EXIT\n");
    
    int running = 1;
    while (running) {
        printf("Enter command: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input < 0 || input > 6) {
            printf("Invalid command\n");
            continue;
        }
        
        if (input == 6) {
            running = 0;
            continue;
        }
        
        Event event = (Event)input;
        
        switch (event) {
            case EVENT_DATA:
                printf("Enter data value: ");
                if (scanf("%d", &data_value) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid data\n");
                    continue;
                }
                break;
            default:
                break;
        }
        
        process_event(event);
    }
    
    printf("Exiting state machine\n");
    return 0;
}