//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4

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

void run_state_machine(state_machine_t *machine, int *inputs, int input_count) {
    if (inputs == NULL || input_count <= 0) {
        return;
    }
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= MAX_INPUTS) {
            continue;
        }
        process_input(machine, inputs[i]);
    }
}

int validate_input_sequence(int *inputs, int input_count) {
    if (inputs == NULL || input_count <= 0) {
        return 0;
    }
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= MAX_INPUTS) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    state_machine_t machine;
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
    
    if (!validate_input_sequence(test_inputs, input_count)) {
        return 1;
    }
    
    run_state_machine(&machine, test_inputs, input_count);
    
    printf("Final state: %d\n", machine.current_state);
    
    int performance_test[1000];
    for (int i = 0; i < 1000; i++) {
        performance_test[i] = i % MAX_INPUTS;
    }
    
    state_machine_t perf_machine;
    initialize_machine(&perf_machine);
    
    add_transition(&perf_machine, STATE_A, 0, STATE_B);
    add_transition(&perf_machine, STATE_A, 1, STATE_C);
    add_transition(&perf_machine, STATE_B, 0, STATE_D);
    add_transition(&perf_machine, STATE_B, 1, STATE_E);
    add_transition(&perf_machine, STATE_C, 0, STATE_F);
    add_transition(&perf_machine, STATE_C, 1, STATE_G);
    add_transition(&perf_machine, STATE_D, 0, STATE_H);
    add_transition(&