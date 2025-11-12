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
    uint64_t counter;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->counter = 0;
    
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

state_t get_next_state(const state_machine_t *machine, state_t current, int input) {
    if (!validate_transition(machine, current, input)) return current;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == current && 
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    return current;
}

void process_state(state_machine_t *machine, int input) {
    if (!validate_transition(machine, machine->current_state, input)) return;
    
    state_t next = get_next_state(machine, machine->current_state, input);
    machine->current_state = next;
    
    for (int i = 0; i < 100; i++) {
        machine->counter += (uint64_t)(machine->current_state + 1) * (input + 1) * i;
        if (machine->counter > UINT64_MAX - 1000) {
            machine->counter = 0;
        }
    }
}

int generate_input(uint64_t iteration) {
    return (int)((iteration * 1103515245 + 12345) % MAX_INPUTS);
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    printf("Initial state: %d\n", (int)machine.current_state);
    printf("Running %d iterations...\n", MAX_ITERATIONS);
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int input = generate_input(i);
        process_state(&machine, input);
        
        if (i % 200000 == 0 && i > 0) {
            printf("Progress: %d/%d iterations, current state: %d\n", 
                   i, MAX_ITERATIONS, (int)machine.current_state);
        }
    }
    
    printf("Final state: %d\n", (int)machine.current_state);
    printf("Final counter value: %lu\n", machine.counter);
    
    return 0;
}