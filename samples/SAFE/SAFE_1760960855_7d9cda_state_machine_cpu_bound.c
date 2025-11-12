//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->num_transitions = 0;
}

static int add_transition(state_machine_t *machine, state_t from, uint8_t input, state_t to) {
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

static state_t process_input(state_machine_t *machine, uint8_t input) {
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

static void build_test_machine(state_machine_t *machine) {
    add_transition(machine, STATE_A, 0, STATE_B);
    add_transition(machine, STATE_A, 1, STATE_C);
    add_transition(machine, STATE_B, 0, STATE_D);
    add_transition(machine, STATE_B, 1, STATE_E);
    add_transition(machine, STATE_C, 0, STATE_F);
    add_transition(machine, STATE_C, 1, STATE_G);
    add_transition(machine, STATE_D, 0, STATE_H);
    add_transition(machine, STATE_D, 1, STATE_A);
    add_transition(machine, STATE_E, 0, STATE_B);
    add_transition(machine, STATE_E, 1, STATE_C);
    add_transition(machine, STATE_F, 0, STATE_D);
    add_transition(machine, STATE_F, 1, STATE_E);
    add_transition(machine, STATE_G, 0, STATE_F);
    add_transition(machine, STATE_G, 1, STATE_G);
    add_transition(machine, STATE_H, 0, STATE_H);
    add_transition(machine, STATE_H, 1, STATE_A);
}

static const char* state_name(state_t state) {
    static const char* names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D",
        "STATE_E", "STATE_F", "STATE_G", "STATE_H"
    };
    if (state < MAX_STATES) {
        return names[state];
    }
    return "INVALID";
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    build_test_machine(&machine);
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_name(machine.current_state));
    
    uint32_t iterations = 1000000;
    uint64_t total_transitions = 0;
    
    for (uint32_t i = 0; i < iterations; i++) {
        uint8_t input = (uint8_t)(i % MAX_INPUTS);
        state_t previous_state = machine.current_state;
        state_t new_state = process_input(&machine, input);
        
        if (previous_state != new_state) {
            total_transitions++;
        }
        
        if (i < 10 || i >= iterations - 10) {
            printf("Step %u: Input=%u, State=%s\n", i, input, state_name(new_state));
        }
    }
    
    printf("Total transitions: %lu\n", total_transitions);
    printf("Final state: %s\n", state_name(machine.current_state));
    
    return 0;
}