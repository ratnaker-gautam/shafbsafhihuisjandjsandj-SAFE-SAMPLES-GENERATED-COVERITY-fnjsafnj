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
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef void (*StateHandler)(void *);

struct StateMachine {
    State current_state;
    int data_value;
    int result;
};

void handle_idle(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: IDLE - Waiting for start event\n");
    machine->data_value = 0;
    machine->result = 0;
}

void handle_ready(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: READY - Enter data value (1-100): ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        machine->current_state = STATE_ERROR;
        return;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' || value < 1 || value > 100) {
        printf("Invalid input. Must be integer 1-100\n");
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->data_value = (int)value;
    printf("Data value %d accepted\n", machine->data_value);
}

void handle_processing(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: PROCESSING - Calculating result...\n");
    
    if (machine->data_value <= 0 || machine->data_value > 100) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->result = machine->data_value * machine->data_value;
    
    if (machine->data_value > 10 && machine->data_value % 7 == 0) {
        printf("Simulating processing failure\n");
        machine->current_state = STATE_ERROR;
    } else {
        printf("Processing completed successfully\n");
    }
}

void handle_done(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: DONE - Final result: %d\n", machine->result);
}

void handle_error(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: ERROR - An error occurred\n");
    machine->result = -1;
}

static StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

void process_event(struct StateMachine *machine, Event event) {
    State next_state = machine->current_state;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) next_state = STATE_READY;
            break;
        case STATE_READY:
            if (event == EVENT_DATA) next_state = STATE_PROCESSING;
            else if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) next_state = STATE_DONE;
            else if (event == EVENT_FAILURE) next_state = STATE_ERROR;
            else if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_DONE:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) next_state = STATE_IDLE;
            break;
    }
    
    if (next_state != machine->current_state) {
        machine->current_state = next_state;
        if (machine->current_state >= STATE_IDLE && machine->current_state <= STATE_ERROR) {
            state_handlers[machine->current_state](machine);
        }
    }
}

int main(void) {
    struct StateMachine machine;
    machine.current_state = STATE_IDLE;
    machine.data_value = 0;
    machine.result = 0;
    
    state_handlers[STATE_IDLE](&machine);
    
    char input[32];
    int running = 1;
    
    while (running) {
        printf("\nAvailable events:\n");
        printf("1 - START, 2 - DATA, 3 - PROCESS, 4 - SUCCESS, 5 - FAILURE, 6 - RESET, 0 - EXIT\n");
        printf("Choose event: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            break;
        }
        
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input\n");
            continue;
        }
        
        Event event;
        switch (choice) {
            case 0: running = 0; continue;
            case 1: event = EVENT_START; break;
            case 2: event = EVENT_DATA; break;
            case 3: event = EVENT_PROCESS; break;
            case 4: event = EVENT_SUCCESS; break;
            case 5: event = EVENT_FAILURE; break;
            case 6: event = EVENT_RESET