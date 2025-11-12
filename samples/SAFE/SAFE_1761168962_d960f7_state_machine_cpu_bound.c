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
    if (input < 0 || input >= MAX_INPUTS) return machine->current_state;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

void run_state_machine(state_machine_t *machine, int num_steps) {
    if (num_steps <= 0 || num_steps > MAX_STEPS) num_steps = 100;
    
    unsigned int seed = 42;
    
    for (int i = 0; i < num_steps; i++) {
        int input = rand_r(&seed) % MAX_INPUTS;
        state_t next_state = process_transition(machine, input);
        machine->current_state = next_state;
        machine->step_count++;
        
        int computation = 0;
        for (int j = 0; j < 1000; j++) {
            computation += (input * j + (int)next_state) % 100;
        }
    }
}

const char* state_to_string(state_t state) {
    switch(state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        default: return "UNKNOWN";
    }
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Initial state: %s\n", state_to_string(machine.current_state));
    printf("Running state machine for 500 steps...\n");
    
    run_state_machine(&machine, 500);
    
    printf("Final state: %s\n", state_to_string(machine.current_state));
    printf("Total steps processed: %d\n", machine.step_count);
    
    int state_counts[MAX_STATES] = {0};
    state_machine_t test_machine;
    initialize_machine(&test_machine);
    
    unsigned int test_seed = 123;
    for (int i = 0; i < 10000; i++) {
        int input = rand_r(&test_seed) % MAX_INPUTS;
        state_t next_state = process_transition(&test_machine, input);
        test_machine.current_state = next_state;
        
        if (next_state >= 0 && next_state < MAX_STATES) {
            state_counts[next_state]++;
        }
        
        int heavy_computation = 0;
        for (int j = 0; j < 500; j++) {
            heavy_computation += (input * j + (int)next_state * i) % 1000;
        }
    }
    
    printf("State distribution after 10000 steps:\n");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("State %s: %d visits\n", state_to_string((state_t)i), state_counts[i]);
    }
    
    return 0;
}