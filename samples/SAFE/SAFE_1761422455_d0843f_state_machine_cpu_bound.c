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

void simulate_workload(int iterations) {
    volatile uint64_t result = 0;
    for (int i = 0; i < iterations; i++) {
        result = (result * 1103515245 + 12345) & 0x7FFFFFFF;
        for (int j = 0; j < 100; j++) {
            result ^= (result >> 3) + (result << 5);
        }
    }
}

int main(void) {
    state_machine_t fsm;
    initialize_machine(&fsm);
    
    add_transition(&fsm, STATE_A, 0, STATE_B);
    add_transition(&fsm, STATE_A, 1, STATE_C);
    add_transition(&fsm, STATE_B, 0, STATE_D);
    add_transition(&fsm, STATE_B, 1, STATE_E);
    add_transition(&fsm, STATE_C, 0, STATE_F);
    add_transition(&fsm, STATE_C, 1, STATE_G);
    add_transition(&fsm, STATE_D, 0, STATE_H);
    add_transition(&fsm, STATE_D, 1, STATE_A);
    add_transition(&fsm, STATE_E, 0, STATE_B);
    add_transition(&fsm, STATE_E, 1, STATE_C);
    add_transition(&fsm, STATE_F, 0, STATE_D);
    add_transition(&fsm, STATE_F, 1, STATE_E);
    add_transition(&fsm, STATE_G, 0, STATE_F);
    add_transition(&fsm, STATE_G, 1, STATE_G);
    add_transition(&fsm, STATE_H, 0, STATE_H);
    add_transition(&fsm, STATE_H, 1, STATE_A);
    
    printf("State machine initialized. Starting simulation...\n");
    
    int input_sequence[] = {0, 1, 0, 1, 1, 0, 0, 1, 0, 1};
    int sequence_length = sizeof(input_sequence) / sizeof(input_sequence[0]);
    
    for (int cycle = 0; cycle < 1000; cycle++) {
        for (int i = 0; i < sequence_length; i++) {
            if (input_sequence[i] >= 0 && input_sequence[i] < MAX_INPUTS) {
                state_t new_state = process_input(&fsm, input_sequence[i]);
                simulate_workload(100);
            }
        }
        
        if (cycle % 100 == 0) {
            printf("Cycle %d completed. Current state: %d\n", cycle, fsm.current_state);
        }
    }
    
    printf("Final state: %d\n", fsm.current_state);
    printf("Simulation completed successfully.\n");
    
    return 0;
}