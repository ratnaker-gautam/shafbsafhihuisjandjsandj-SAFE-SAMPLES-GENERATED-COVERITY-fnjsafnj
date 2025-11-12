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
        machine->current_state = STATE_ERROR;
        return;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' || value < 1 || value > 100) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->data_value = (int)value;
    printf("Data value %d accepted\n", machine->data_value);
}

void handle_processing(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: PROCESSING - Processing data value %d\n", machine->data_value);
    
    if (machine->data_value < 0) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    long temp = (long)machine->data_value * 2L;
    if (temp > 1000) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->result = (int)temp;
}

void handle_done(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: DONE - Result: %d\n", machine->result);
}

void handle_error(void *sm) {
    struct StateMachine *machine = (struct StateMachine *)sm;
    printf("State: ERROR - An error occurred\n");
    machine->data_value = 0;
    machine->result = -1;
}

StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

State transition_table[5][6] = {
    {STATE_READY,     STATE_IDLE, STATE_IDLE,      STATE_IDLE, STATE_IDLE, STATE_IDLE},
    {STATE_READY,     STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_DONE, STATE_ERROR, STATE_IDLE},
    {STATE_DONE,      STATE_DONE, STATE_DONE,      STATE_DONE, STATE_DONE, STATE_IDLE},
    {STATE_ERROR,     STATE_ERROR, STATE_ERROR,    STATE_ERROR, STATE_ERROR, STATE_IDLE}
};

void process_event(struct StateMachine *machine, Event event) {
    if (machine == NULL || event < EVENT_START || event > EVENT_RESET) {
        return;
    }
    
    State new_state = transition_table[machine->current_state][event];
    if (new_state < STATE_IDLE || new_state > STATE_ERROR) {
        machine->current_state = STATE_ERROR;
        return;
    }
    
    machine->current_state = new_state;
    
    if (machine->current_state >= STATE_IDLE && machine->current_state <= STATE_ERROR) {
        state_handlers[machine->current_state](machine);
    }
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    struct StateMachine machine;
    machine.current_state = STATE_IDLE;
    machine.data_value = 0;
    machine.result = 0;
    
    state_handlers[STATE_IDLE](&machine);
    
    int running = 1;
    while (running) {
        print_menu();
        
        char input[32];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        if (choice < 0 || choice > 5) {
            printf("Invalid event\n");
            continue;
        }
        
        process_event(&machine, (Event)choice);
    }
    
    printf("State machine terminated\n");
    return 0;
}