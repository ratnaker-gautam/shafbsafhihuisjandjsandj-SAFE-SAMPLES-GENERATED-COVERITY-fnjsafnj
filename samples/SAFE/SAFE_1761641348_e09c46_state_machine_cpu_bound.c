//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

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

typedef struct {
    state_t current_state;
    uint8_t input;
    state_t next_state;
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

int add_transition(state_machine_t *machine, state_t from, uint8_t input, state_t to) {
    if (machine->num_transitions >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    
    machine->transitions[machine->num_transitions].current_state = from;
    machine->transitions[machine->num_transitions].input = input;
    machine->transitions[machine->num_transitions].next_state = to;
    machine->num_transitions++;
    return 1;
}

state_t process_input(state_machine_t *machine, uint8_t input) {
    if (input >= MAX_INPUTS) {
        return machine->current_state;
    }
    
    for (size_t i = 0; i < machine->num_transitions; i++) {
        if (machine->transitions[i].current_state == machine->current_state &&
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next_state;
            break;
        }
    }
    
    return machine->current_state;
}

void generate_test_inputs(uint8_t *inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (uint8_t)(i % MAX_INPUTS);
    }
}

int main(void) {
    state_machine_t machine;
    uint8_t test_inputs[1000];
    size_t num_iterations = 1000000;
    
    initialize_machine(&machine);
    
    if (!add_transition(&machine, STATE_A, 0, STATE_B) ||
        !add_transition(&machine, STATE_B, 1, STATE_C) ||
        !add_transition(&machine, STATE_C, 2, STATE_D) ||
        !add_transition(&machine, STATE_D, 3, STATE_E) ||
        !add_transition(&machine, STATE_E, 4, STATE_F) ||
        !add_transition(&machine, STATE_F, 5, STATE_G) ||
        !add_transition(&machine, STATE_G, 6, STATE_H) ||
        !add_transition(&machine, STATE_H, 7, STATE_A) ||
        !add_transition(&machine, STATE_A, 8, STATE_C) ||
        !add_transition(&machine, STATE_B, 9, STATE_D) ||
        !add_transition(&machine, STATE_C, 10, STATE_E) ||
        !add_transition(&machine, STATE_D, 11, STATE_F) ||
        !add_transition(&machine, STATE_E, 12, STATE_G) ||
        !add_transition(&machine, STATE_F, 13, STATE_H) ||
        !add_transition(&machine, STATE_G, 14, STATE_A) ||
        !add_transition(&machine, STATE_H, 15, STATE_B)) {
        return EXIT_FAILURE;
    }
    
    generate_test_inputs(test_inputs, sizeof(test_inputs)/sizeof(test_inputs[0]));
    
    state_t final_state = machine.current_state;
    
    for (size_t iter = 0; iter < num_iterations; iter++) {
        for (size_t i = 0; i < sizeof(test_inputs)/sizeof(test_inputs[0]); i++) {
            final_state = process_input(&machine, test_inputs[i]);
        }
        
        if (iter % 1000 == 0) {
            machine.current_state = STATE_A;
        }
    }
    
    printf("Final state: %d\n", (int)final_state);
    printf("Total state transitions processed: %zu\n", 
           num_iterations * sizeof(test_inputs)/sizeof(test_inputs[0]));
    
    return EXIT_SUCCESS;
}