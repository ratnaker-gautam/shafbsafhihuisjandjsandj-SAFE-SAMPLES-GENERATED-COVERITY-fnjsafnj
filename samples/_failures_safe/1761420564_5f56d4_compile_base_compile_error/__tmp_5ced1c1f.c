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

typedef void (*StateHandler)(void*);

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
    printf("Data accepted: %d\n", machine->data_value);
}

void handle_processing(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: PROCESSING - Calculating...\n");
    
    if (machine->data_value <= 0) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    int temp = machine->data_value * 2;
    if (temp < machine->data_value) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->result = temp;
    printf("Calculation complete: %d * 2 = %d\n", machine->data_value, machine->result);
}

void handle_done(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: DONE - Result: %d\n", machine->result);
    printf("Press 'r' to reset or 'q' to quit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        machine->current_state = STATE_IDLE;
        return;
    }
    
    if (input[0] == 'q' || input[0] == 'Q') {
        exit(0);
    }
}

void handle_error(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: ERROR - An error occurred\n");
    printf("Press 'r' to reset: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        machine->current_state = STATE_IDLE;
        return;
    }
    
    if (input[0] == 'r' || input[0] == 'R') {
        machine->current_state = STATE_IDLE;
    }
}

StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

void process_event(struct StateMachine *machine, Event event) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                machine->current_state = STATE_READY;
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                machine->current_state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                machine->current_state = STATE_IDLE;
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                machine->current_state = STATE_DONE;
            } else if (event == EVENT_FAILURE) {
                machine->current_state = STATE_ERROR;
            }
            break;
        case STATE_DONE:
            if (event == EVENT_RESET) {
                machine->current_state = STATE_IDLE;
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                machine->current_state = STATE_IDLE;
            }
            break;
    }
}

int main(void) {
    struct StateMachine machine;
    machine.current_state = STATE_IDLE;
    machine.data_value = 0;
    machine.result = 0;
    
    printf("State Machine Demo - Data Processor\n");
    printf("Commands: s=start, p=process, r=reset, q=quit\n");
    
    while (1) {
        if (machine.current_state >= STATE_IDLE && machine.current_state <= STATE_ERROR) {
            state_handlers[machine.current_state](&machine);
        } else {
            machine.current_state = STATE_ERROR;
            continue;
        }
        
        printf("Enter command: ");
        char input[32];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        Event event;
        switch (input[0]) {
            case 's':
            case 'S':
                event = EVENT_START;
                break;
            case 'p':
            case 'P':
                event