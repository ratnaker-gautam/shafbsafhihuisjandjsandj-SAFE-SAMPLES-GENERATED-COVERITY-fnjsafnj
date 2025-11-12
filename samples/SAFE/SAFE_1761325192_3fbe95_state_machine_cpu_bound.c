//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4
#define MAX_STEPS 1000

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
    int step_count;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->step_count = 0;
    
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
    if (input < 0 || input >= MAX_INPUTS) {
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

void run_state_machine(state_machine_t *machine, int num_steps) {
    if (num_steps <= 0 || num_steps > MAX_STEPS) {
        num_steps = 100;
    }
    
    unsigned int seed = 42;
    
    for (int i = 0; i < num_steps; i++) {
        int input = rand_r(&seed) % MAX_INPUTS;
        state_t next_state = process_transition(machine, input);
        machine->current_state = next_state;
        machine->step_count++;
        
        long computation = 0;
        for (int j = 0; j < 1000; j++) {
            computation += (j * input + machine->current_state) % 100;
        }
    }
}

void print_state_info(state_machine_t *machine) {
    const char *state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D",
        "STATE_E", "STATE_F", "STATE_G", "STATE_H"
    };
    
    int state_index = machine->current_state;
    if (state_index < 0 || state_index >= MAX_STATES) {
        state_index = 0;
    }
    
    printf("Final state: %s\n", state_names[state_index]);
    printf("Total steps executed: %d\n", machine->step_count);
    
    int state_counts[MAX_STATES] = {0};
    state_machine_t temp_machine;
    initialize_machine(&temp_machine);
    
    unsigned int seed = 42;
    for (int i = 0; i < 100; i++) {
        int input = rand_r(&seed) % MAX_INPUTS;
        state_t next_state = process_transition(&temp_machine, input);
        temp_machine.current_state = next_state;
        
        if (next_state >= 0 && next_state < MAX_STATES) {
            state_counts[next_state]++;
        }
    }
    
    printf("State distribution (sample): ");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("%s:%d ", state_names[i], state_counts[i]);
    }
    printf("\n");
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("State Machine Simulation Started\n");
    printf("Running CPU-intensive state transitions...\n");
    
    run_state_machine(&machine, 500);
    
    print_state_info(&machine);
    
    long final_computation = 0;
    for (int i = 0; i < 10000; i++) {
        final_computation = (final_computation * 31 + machine.current_state) % 1000000;
    }
    
    printf("Final computation result: %ld\n", final_computation);
    
    return 0;
}