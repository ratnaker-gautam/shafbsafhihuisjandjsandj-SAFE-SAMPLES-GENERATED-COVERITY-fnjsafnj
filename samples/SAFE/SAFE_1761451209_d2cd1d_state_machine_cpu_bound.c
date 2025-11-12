//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(state_machine_t *machine, state_t from, int input, state_t to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (input < 0 || input >= MAX_INPUTS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

state_t process_input(state_machine_t *machine, int input) {
    if (input < 0 || input >= MAX_INPUTS) {
        return machine->current_state;
    }
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return machine->current_state;
        }
    }
    
    return machine->current_state;
}

void run_state_machine_simulation(state_machine_t *machine, int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        iterations = 1000;
    }
    
    int state_counts[MAX_STATES] = {0};
    int input_sequence[MAX_ITERATIONS];
    
    for (int i = 0; i < iterations; i++) {
        input_sequence[i] = i % MAX_INPUTS;
    }
    
    for (int i = 0; i < iterations; i++) {
        state_t prev_state = machine->current_state;
        state_t new_state = process_input(machine, input_sequence[i]);
        
        if (new_state >= 0 && new_state < MAX_STATES) {
            state_counts[new_state]++;
        }
        
        if (prev_state == new_state) {
            input_sequence[i] = (input_sequence[i] + 1) % MAX_INPUTS;
        }
    }
    
    printf("State distribution after %d iterations:\n", iterations);
    const char *state_names[] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    for (int i = 0; i < MAX_STATES; i++) {
        double percentage = (double)state_counts[i] / iterations * 100.0;
        printf("State %s: %d times (%.2f%%)\n", state_names[i], state_counts[i], percentage);
    }
    printf("Final state: %s\n", state_names[machine->current_state]);
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, 0, STATE_B);
    add_transition(&machine, STATE_A, 1, STATE_C);
    add_transition(&machine, STATE_B, 0, STATE_D);
    add_transition(&machine, STATE_B, 1, STATE_A);
    add_transition(&machine, STATE_C, 0, STATE_A);
    add_transition(&machine, STATE_C, 1, STATE_D);
    add_transition(&machine, STATE_D, 0, STATE_E);
    add_transition(&machine, STATE_D, 1, STATE_F);
    add_transition(&machine, STATE_E, 0, STATE_G);
    add_transition(&machine, STATE_E, 1, STATE_H);
    add_transition(&machine, STATE_F, 0, STATE_H);
    add_transition(&machine, STATE_F, 1, STATE_G);
    add_transition(&machine, STATE_G, 0, STATE_A);
    add_transition(&machine, STATE_G, 1, STATE_B);
    add_transition(&machine, STATE_H, 0, STATE_C);
    add_transition(&machine, STATE_H, 1, STATE_D);
    
    run_state_machine_simulation(&machine, 500000);
    
    return 0;
}