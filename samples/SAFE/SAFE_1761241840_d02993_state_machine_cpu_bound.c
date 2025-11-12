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

int validate_input(int input) {
    return (input >= 0 && input < MAX_INPUTS);
}

state_t process_transition(state_machine_t *machine, int input) {
    if (!validate_input(input)) {
        return machine->current_state;
    }
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    
    return machine->current_state;
}

void run_state_machine(state_machine_t *machine, int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        iterations = 1000;
    }
    
    for (int i = 0; i < iterations; i++) {
        int input = i % MAX_INPUTS;
        state_t next_state = process_transition(machine, input);
        machine->current_state = next_state;
        machine->cycle_count++;
        
        for (int j = 0; j < 100; j++) {
            machine->cycle_count = (machine->cycle_count * 1103515245 + 12345) & 0x7FFFFFFF;
        }
    }
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Initial state: %d\n", machine.current_state);
    printf("Running state machine for 100000 cycles...\n");
    
    run_state_machine(&machine, 100000);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total cycles processed: %lu\n", (unsigned long)machine.cycle_count);
    
    int result = (int)(machine.cycle_count % 1000);
    printf("Computation result: %d\n", result);
    
    return 0;
}