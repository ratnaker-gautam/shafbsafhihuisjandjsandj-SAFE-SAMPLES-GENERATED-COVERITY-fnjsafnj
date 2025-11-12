//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    INPUT_INVALID
} Input;

typedef struct {
    State current;
    Input input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
    uint64_t step_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->step_count = 0;
    
    Transition trans[] = {
        {STATE_A, INPUT_0, STATE_B}, {STATE_A, INPUT_1, STATE_C},
        {STATE_B, INPUT_0, STATE_D}, {STATE_B, INPUT_1, STATE_E},
        {STATE_C, INPUT_0, STATE_F}, {STATE_C, INPUT_1, STATE_G},
        {STATE_D, INPUT_0, STATE_H}, {STATE_D, INPUT_1, STATE_A},
        {STATE_E, INPUT_0, STATE_B}, {STATE_E, INPUT_1, STATE_C},
        {STATE_F, INPUT_0, STATE_D}, {STATE_F, INPUT_1, STATE_E},
        {STATE_G, INPUT_0, STATE_F}, {STATE_G, INPUT_1, STATE_G},
        {STATE_H, INPUT_0, STATE_H}, {STATE_H, INPUT_1, STATE_A}
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

Input get_next_input(uint64_t step) {
    uint64_t pattern = step % 16;
    switch (pattern) {
        case 0: case 3: case 6: case 9: case 12: case 15:
            return INPUT_0;
        case 1: case 4: case 7: case 10: case 13:
            return INPUT_1;
        case 2: case 5: case 8: case 11: case 14:
            return INPUT_2;
        default:
            return INPUT_3;
    }
}

int execute_transition(StateMachine *machine, Input input) {
    if (input == INPUT_INVALID) return 0;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void run_simulation(StateMachine *machine, uint64_t iterations) {
    if (iterations > 1000000000) iterations = 1000000000;
    
    for (uint64_t i = 0; i < iterations; i++) {
        Input input = get_next_input(i);
        if (!execute_transition(machine, input)) {
            break;
        }
        machine->step_count++;
        
        uint64_t result = (uint64_t)machine->current_state * machine->step_count;
        result = result ^ (result >> 32);
        result = result * 0x9e3779b9;
        result = result ^ (result >> 16);
        result = result * 0x85ebca6b;
        result = result ^ (result >> 13);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    printf("Initial state: %d\n", machine.current_state);
    
    uint64_t iterations = 50000000;
    run_simulation(&machine, iterations);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Steps executed: %lu\n", machine.step_count);
    printf("Simulation completed.\n");
    
    return 0;
}