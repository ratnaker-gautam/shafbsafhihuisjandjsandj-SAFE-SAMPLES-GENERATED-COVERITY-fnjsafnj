//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'C', STATE_IDLE},
    {STATE_PROCESSING, 'Q', STATE_COMPLETE},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_ERROR, 'Q', STATE_COMPLETE}
};
static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->buffer_pos = 0;
    machine->result = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

static State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

static void process_buffer(StateMachine *machine) {
    if (machine->buffer_pos == 0) {
        return;
    }
    
    int value = 0;
    for (size_t i = 0; i < machine->buffer_pos; i++) {
        if (machine->buffer[i] == '0') {
            value = (value << 1);
        } else if (machine->buffer[i] == '1') {
            value = (value << 1) | 1;
        }
        if (value < 0) {
            machine->current_state = STATE_ERROR;
            return;
        }
    }
    machine->result += value;
    machine->buffer_pos = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

static void handle_state_action(StateMachine *machine, char input) {
    switch (machine->current_state) {
        case STATE_IDLE:
            break;
        case STATE_READING:
            if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                machine->buffer[machine->buffer_pos++] = input;
            } else {
                machine->current_state = STATE_ERROR;
            }
            break;
        case STATE_PROCESSING:
            process_buffer(machine);
            break;
        case STATE_COMPLETE:
            break;
        case STATE_ERROR:
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, 0/1=Binary digits, E=End input, C=Continue, R=Reset, Q=Quit\n");
    
    int running = 1;
    while (running) {
        printf("Current state: %d, Result: %d\n", machine.current_state, machine.result);
        printf("Enter command: ");
        
        char input[MAX_INPUT_LEN];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }
        
        char command = input[0];
        if (!isprint(command)) {
            continue;
        }
        
        State next_state = get_next_state(machine.current_state, command);
        if (next_state == STATE_ERROR && machine.current_state != STATE_ERROR) {
            printf("Invalid transition from state %d with command '%c'\n", machine.current_state, command);
            machine.current_state = STATE_ERROR;
        } else {
            handle_state_action(&machine, command);
            machine.current_state = next_state;
        }
        
        if (machine.current_state == STATE_COMPLETE) {
            running = 0;
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("Error state reached. Use R to reset or Q to quit.\n");
        }
    }
    
    printf("Final result: %d\n", machine.result);
    return 0;
}