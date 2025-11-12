//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    if (input < 0 || input >= MAX_INPUTS) {
        return 0;
    }
    
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, int input) {
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

void generate_deterministic_input(int *inputs, int count) {
    for (int i = 0; i < count; i++) {
        inputs[i] = (i * 7 + 3) % MAX_INPUTS;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    if (!add_transition(&machine, STATE_A, 0, STATE_B)) return 1;
    if (!add_transition(&machine, STATE_A, 1, STATE_C)) return 1;
    if (!add_transition(&machine, STATE_B, 0, STATE_D)) return 1;
    if (!add_transition(&machine, STATE_B, 1, STATE_E)) return 1;
    if (!add_transition(&machine, STATE_C, 0, STATE_F)) return 1;
    if (!add_transition(&machine, STATE_C, 1, STATE_G)) return 1;
    if (!add_transition(&machine, STATE_D, 0, STATE_H)) return 1;
    if (!add_transition(&machine, STATE_D, 1, STATE_A)) return 1;
    if (!add_transition(&machine, STATE_E, 0, STATE_B)) return 1;
    if (!add_transition(&machine, STATE_E, 1, STATE_C)) return 1;
    if (!add_transition(&machine, STATE_F, 0, STATE_D)) return 1;
    if (!add_transition(&machine, STATE_F, 1, STATE_E)) return 1;
    if (!add_transition(&machine, STATE_G, 0, STATE_F)) return 1;
    if (!add_transition(&machine, STATE_G, 1, STATE_G)) return 1;
    if (!add_transition(&machine, STATE_H, 0, STATE_H)) return 1;
    if (!add_transition(&machine, STATE_H, 1, STATE_A)) return 1;
    
    int iterations = MAX_ITERATIONS;
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 1;
    }
    
    int *inputs = malloc(iterations * sizeof(int));
    if (inputs == NULL) {
        return 1;
    }
    
    generate_deterministic_input(inputs, iterations);
    
    State final_state;
    for (int i = 0; i < iterations; i++) {
        final_state = process_input(&machine, inputs[i]);
    }
    
    printf("Final state: %d\n", final_state);
    printf("Total transitions processed: %d\n", iterations);
    
    free(inputs);
    return 0;
}