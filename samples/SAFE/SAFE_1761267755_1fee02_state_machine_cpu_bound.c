//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 64

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

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} Input;

typedef struct {
    State current;
    Input input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    size_t transition_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(StateMachine *machine, State from, Input input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, Input input) {
    if (input >= MAX_INPUTS) {
        return machine->current_state;
    }
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            break;
        }
    }
    return machine->current_state;
}

void simulate_machine(StateMachine *machine, Input *inputs, size_t input_count) {
    if (inputs == NULL || input_count == 0) {
        return;
    }
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= MAX_INPUTS) {
            continue;
        }
        process_input(machine, inputs[i]);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, INPUT_0, STATE_B);
    add_transition(&machine, STATE_A, INPUT_1, STATE_C);
    add_transition(&machine, STATE_B, INPUT_0, STATE_D);
    add_transition(&machine, STATE_B, INPUT_1, STATE_E);
    add_transition(&machine, STATE_C, INPUT_0, STATE_F);
    add_transition(&machine, STATE_C, INPUT_1, STATE_G);
    add_transition(&machine, STATE_D, INPUT_0, STATE_H);
    add_transition(&machine, STATE_D, INPUT_1, STATE_A);
    add_transition(&machine, STATE_E, INPUT_0, STATE_B);
    add_transition(&machine, STATE_E, INPUT_1, STATE_C);
    add_transition(&machine, STATE_F, INPUT_0, STATE_D);
    add_transition(&machine, STATE_F, INPUT_1, STATE_E);
    add_transition(&machine, STATE_G, INPUT_0, STATE_F);
    add_transition(&machine, STATE_G, INPUT_1, STATE_G);
    add_transition(&machine, STATE_H, INPUT_0, STATE_H);
    add_transition(&machine, STATE_H, INPUT_1, STATE_A);
    
    Input test_inputs[] = {INPUT_0, INPUT_1, INPUT_0, INPUT_1, INPUT_0, INPUT_1, INPUT_0, INPUT_1};
    size_t input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    simulate_machine(&machine, test_inputs, input_count);
    
    printf("Final state: %d\n", (int)machine.current_state);
    
    return 0;
}