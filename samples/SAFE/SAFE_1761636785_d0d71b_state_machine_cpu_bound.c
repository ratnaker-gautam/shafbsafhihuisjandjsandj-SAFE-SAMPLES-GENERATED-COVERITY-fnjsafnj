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
            machine->transitions[machine->transition_count++] = trans[i];
        }
    }
}

int process_transition(state_machine_t *machine, int input) {
    if (input < 0 || input >= MAX_INPUTS) return 0;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            machine->cycle_count++;
            return 1;
        }
    }
    return 0;
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

void run_state_machine_cycles(state_machine_t *machine, int cycles) {
    if (cycles <= 0 || cycles > MAX_ITERATIONS) cycles = 1000;
    
    for (int i = 0; i < cycles; i++) {
        int input = i % MAX_INPUTS;
        if (!process_transition(machine, input)) {
            break;
        }
        
        if (machine->cycle_count >= MAX_ITERATIONS) {
            break;
        }
    }
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Initial state: %s\n", state_to_string(machine.current_state));
    
    int cycles;
    printf("Enter number of cycles to run (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &cycles) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cycles <= 0 || cycles > MAX_ITERATIONS) {
        printf("Invalid cycle count\n");
        return 1;
    }
    
    run_state_machine_cycles(&machine, cycles);
    
    printf("Final state: %s\n", state_to_string(machine.current_state));
    printf("Total transitions: %lu\n", (unsigned long)machine.cycle_count);
    
    return 0;
}