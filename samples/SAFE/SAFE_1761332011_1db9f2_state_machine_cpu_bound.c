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
    state_t current_state;
    int input;
    state_t next_state;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    
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

state_t process_transition(state_machine_t *machine, int input) {
    if (input < 0 || input >= MAX_INPUTS) return machine->current_state;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current_state == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next_state;
        }
    }
    
    return machine->current_state;
}

int validate_iterations(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) return MAX_ITERATIONS;
    return iterations;
}

int generate_deterministic_input(int iteration, int state) {
    return (iteration + state) % MAX_INPUTS;
}

void run_state_machine(state_machine_t *machine, int iterations) {
    int valid_iterations = validate_iterations(iterations);
    int state_counts[MAX_STATES] = {0};
    
    for (int i = 0; i < valid_iterations; i++) {
        int input = generate_deterministic_input(i, machine->current_state);
        state_t next_state = process_transition(machine, input);
        
        if (next_state >= 0 && next_state < MAX_STATES) {
            machine->current_state = next_state;
            state_counts[machine->current_state]++;
        }
    }
    
    printf("State machine completed %d iterations\n", valid_iterations);
    printf("Final state distribution:\n");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("State %d: %d visits\n", i, state_counts[i]);
    }
}

int main(int argc, char *argv[]) {
    int iterations = 100000;
    
    if (argc > 1) {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && *endptr == '\0' && val >= 0 && val <= INT_MAX) {
            iterations = (int)val;
        }
    }
    
    state_machine_t machine;
    initialize_machine(&machine);
    run_state_machine(&machine, iterations);
    
    return 0;
}