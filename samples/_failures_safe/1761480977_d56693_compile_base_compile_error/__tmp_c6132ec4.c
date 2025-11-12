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
} state_t;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} input_t;

typedef struct {
    state_t current;
    input_t input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    size_t num_transitions;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->num_transitions = 0;
}

int add_transition(state_machine_t *machine, state_t from, input_t input, state_t to) {
    if (machine->num_transitions >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    
    machine->transitions[machine->num_transitions].current = from;
    machine->transitions[machine->num_transitions].input = input;
    machine->transitions[machine->num_transitions].next = to;
    machine->num_transitions++;
    return 1;
}

state_t process_input(state_machine_t *machine, input_t input) {
    if (input >= MAX_INPUTS) {
        return machine->current_state;
    }
    
    for (size_t i = 0; i < machine->num_transitions; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            break;
        }
    }
    
    return machine->current_state;
}

void run_state_machine_simulation(state_machine_t *machine, input_t *inputs, size_t num_inputs) {
    if (inputs == NULL || num_inputs == 0) {
        return;
    }
    
    for (size_t i = 0; i < num_inputs; i++) {
        if (inputs[i] >= MAX_INPUTS) {
            continue;
        }
        process_input(machine, inputs[i]);
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, INPUT_0, STATE_B);
    add_transition(&machine, STATE_A, INPUT_1, STATE_C);
    add_transition(&machine, STATE_B, INPUT_0, STATE_C);
    add_transition(&machine, STATE_B, INPUT_1, STATE_D);
    add_transition(&machine, STATE_C, INPUT_0, STATE_D);
    add_transition(&machine, STATE_C, INPUT_1, STATE_E);
    add_transition(&machine, STATE_D, INPUT_0, STATE_E);
    add_transition(&machine, STATE_D, INPUT_1, STATE_F);
    add_transition(&machine, STATE_E, INPUT_0, STATE_F);
    add_transition(&machine, STATE_E, INPUT_1, STATE_G);
    add_transition(&machine, STATE_F, INPUT_0, STATE_G);
    add_transition(&machine, STATE_F, INPUT_1, STATE_H);
    add_transition(&machine, STATE_G, INPUT_0, STATE_H);
    add_transition(&machine, STATE_G, INPUT_1, STATE_A);
    add_transition(&machine, STATE_H, INPUT_0, STATE_A);
    add_transition(&machine, STATE_H, INPUT_1, STATE_B);
    
    input_t test_inputs[] = {INPUT_0, INPUT_1, INPUT_0, INPUT_1, INPUT_0, INPUT_1, INPUT_0, INPUT_1};
    size_t num_test_inputs = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    printf("Initial state: %d\n", machine.current_state);
    
    for (size_t i = 0; i < num_test_inputs; i++) {
        state_t new_state = process_input(&machine, test_inputs[i]);
        printf("Input %zu: %d -> State: %d\n", i, test_inputs[i], new_state);
    }
    
    state_machine_t machine2;
    initialize_machine(&machine2);
    
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_INPUTS; j++) {
            state_t next_state = (state_t)((i + j + 1) % MAX_STATES);
            add_transition(&machine2, (state_t)i, (input_t)j, next_state);
        }
    }
    
    input_t complex_inputs[1000];
    for (size_t i = 0; i < 1000; i++) {
        complex_inputs[i] = (input_t)(i % MAX_INPUTS);
    }
    
    run_state_machine_simulation(&machine2, complex_inputs, 1000);
    printf("Final state after 1000 transitions: %d\n", machine2.current_state);
    
    return 0