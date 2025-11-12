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

int generate_input(int iteration) {
    return (iteration * 7 + 13) % MAX_INPUTS;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    printf("Initial state: %d\n", machine.current_state);
    
    uint64_t state_visits[MAX_STATES] = {0};
    int valid_iterations = 0;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i >= INT_MAX - 1) break;
        
        int input = generate_input(i);
        if (input < 0 || input >= MAX_INPUTS) continue;
        
        state_t old_state = machine.current_state;
        state_t new_state = process_transition(&machine, input);
        
        if (old_state >= 0 && old_state < MAX_STATES) {
            state_visits[old_state]++;
        }
        
        machine.current_state = new_state;
        valid_iterations++;
    }
    
    printf("Completed %d iterations\n", valid_iterations);
    printf("State visit counts:\n");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("State %d: %lu visits\n", i, state_visits[i]);
    }
    printf("Final state: %d\n", machine.current_state);
    
    return 0;
}