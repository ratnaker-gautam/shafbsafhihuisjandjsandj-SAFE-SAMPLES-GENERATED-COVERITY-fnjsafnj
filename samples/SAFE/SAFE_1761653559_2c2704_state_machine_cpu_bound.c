//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4
#define MAX_ITERATIONS 1000000

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
    state_t current;
    int input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
    uint64_t cycle_count;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->cycle_count = 0;
    
    transition_t trans[] = {
        {STATE_A, 0, STATE_B}, {STATE_A, 1, STATE_C},
        {STATE_B, 0, STATE_D}, {STATE_B, 1, STATE_E},
        {STATE_C, 0, STATE_F}, {STATE_C, 1, STATE_G},
        {STATE_D, 0, STATE_H}, {STATE_D, 1, STATE_A},
        {STATE_E, 0, STATE_B}, {STATE_E, 1, STATE_C},
        {STATE_F, 0, STATE_D}, {STATE_F, 1, STATE_E},
        {STATE_G, 0, STATE_F}, {STATE_G, 1, STATE_G},
        {STATE_H, 0, STATE_H}, {STATE_H, 1, STATE_A}
    };
    
    int count = sizeof(trans) / sizeof(trans[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = trans[i];
            machine->transition_count++;
        }
    }
}

int validate_transition(const state_machine_t *machine, state_t current, int input) {
    if (input < 0 || input >= MAX_INPUTS) return 0;
    if (current < STATE_A || current > STATE_H) return 0;
    return 1;
}

state_t execute_transition(state_machine_t *machine, int input) {
    if (!validate_transition(machine, machine->current_state, input)) {
        return machine->current_state;
    }
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            machine->cycle_count++;
            return machine->current_state;
        }
    }
    
    return machine->current_state;
}

void generate_deterministic_input(int *input_sequence, int length) {
    if (length <= 0) return;
    
    for (int i = 0; i < length; i++) {
        input_sequence[i] = (i * 7 + 3) % MAX_INPUTS;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    int input_sequence[1000];
    int sequence_length = 1000;
    
    if (sequence_length > 1000) sequence_length = 1000;
    generate_deterministic_input(input_sequence, sequence_length);
    
    printf("Starting state machine simulation...\n");
    printf("Initial state: %d\n", machine.current_state);
    
    uint64_t total_transitions = 0;
    int max_iterations = MAX_ITERATIONS;
    if (max_iterations < 0) max_iterations = 1000000;
    
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        int input_idx = iteration % sequence_length;
        if (input_idx < 0 || input_idx >= sequence_length) input_idx = 0;
        
        int input = input_sequence[input_idx];
        if (input < 0 || input >= MAX_INPUTS) input = 0;
        
        state_t previous_state = machine.current_state;
        state_t new_state = execute_transition(&machine, input);
        
        if (iteration < 10) {
            printf("Step %d: State %d -> %d (input: %d)\n", 
                   iteration, previous_state, new_state, input);
        }
        
        total_transitions++;
        
        if (machine.cycle_count > 0 && machine.cycle_count % 100000 == 0) {
            if (iteration % 100000 == 0) {
                printf("Completed %lu cycles\n", (unsigned long)machine.cycle_count);
            }
        }
        
        if (total_transitions >= 1000000) break;
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total transitions: %lu\n", (unsigned long)total_transitions);
    printf("Total cycles: %lu\n", (unsigned long)machine.cycle_count);
    
    return 0;
}