//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 128

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    int input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(StateMachine *machine, State from, int input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from < 0 || from >= MAX_STATES || to < 0 || to >= MAX_STATES) {
        return 0;
    }
    if (input < 0 || input >= MAX_INPUTS) {
        return 0;
    }
    
    Transition *t = &machine->transitions[machine->transition_count];
    t->current = from;
    t->input = input;
    t->next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, int input) {
    if (input < 0 || input >= MAX_INPUTS) {
        return machine->current_state;
    }
    
    for (int i = 0; i < machine->transition_count; i++) {
        Transition *t = &machine->transitions[i];
        if (t->current == machine->current_state && t->input == input) {
            machine->current_state = t->next;
            break;
        }
    }
    
    return machine->current_state;
}

void simulate_state_machine(StateMachine *machine, int *inputs, int input_count) {
    if (inputs == NULL || input_count <= 0) {
        return;
    }
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= MAX_INPUTS) {
            continue;
        }
        State new_state = process_input(machine, inputs[i]);
        printf("Input: %d -> State: %d\n", inputs[i], new_state);
    }
}

int main() {
    StateMachine machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, 0, STATE_B);
    add_transition(&machine, STATE_A, 1, STATE_C);
    add_transition(&machine, STATE_B, 0, STATE_D);
    add_transition(&machine, STATE_B, 1, STATE_E);
    add_transition(&machine, STATE_C, 0, STATE_F);
    add_transition(&machine, STATE_C, 1, STATE_G);
    add_transition(&machine, STATE_D, 0, STATE_H);
    add_transition(&machine, STATE_D, 1, STATE_A);
    add_transition(&machine, STATE_E, 0, STATE_B);
    add_transition(&machine, STATE_E, 1, STATE_C);
    add_transition(&machine, STATE_F, 0, STATE_D);
    add_transition(&machine, STATE_F, 1, STATE_E);
    add_transition(&machine, STATE_G, 0, STATE_F);
    add_transition(&machine, STATE_G, 1, STATE_G);
    add_transition(&machine, STATE_H, 0, STATE_H);
    add_transition(&machine, STATE_H, 1, STATE_A);
    
    int test_inputs[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    printf("Starting state machine simulation...\n");
    printf("Initial state: %d\n", machine.current_state);
    
    simulate_state_machine(&machine, test_inputs, input_count);
    
    printf("Final state: %d\n", machine.current_state);
    
    return 0;
}