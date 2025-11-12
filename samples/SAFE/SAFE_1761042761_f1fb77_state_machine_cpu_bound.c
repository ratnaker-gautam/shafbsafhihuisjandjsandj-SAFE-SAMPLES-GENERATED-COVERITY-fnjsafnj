//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

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
    state_t current_state;
    uint8_t input;
    state_t next_state;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    size_t transition_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
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
    
    size_t count = sizeof(trans) / sizeof(trans[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (size_t i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = trans[i];
            machine->transition_count++;
        }
    }
}

static state_t process_transition(state_machine_t *machine, uint8_t input) {
    if (input >= MAX_INPUTS) return machine->current_state;
    
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current_state == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next_state;
        }
    }
    
    return machine->current_state;
}

static void simulate_state_machine(state_machine_t *machine, uint8_t *inputs, size_t input_count) {
    if (!inputs || input_count == 0) return;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= MAX_INPUTS) continue;
        
        state_t old_state = machine->current_state;
        machine->current_state = process_transition(machine, inputs[i]);
        
        printf("Input: %u, State: %d -> %d\n", 
               inputs[i], old_state, machine->current_state);
    }
}

static void generate_test_inputs(uint8_t *inputs, size_t count) {
    if (!inputs || count == 0) return;
    
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (uint8_t)(i % MAX_INPUTS);
    }
}

static const char* state_name(state_t state) {
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

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    const size_t input_count = 32;
    uint8_t inputs[input_count];
    generate_test_inputs(inputs, input_count);
    
    printf("Initial state: %s\n", state_name(machine.current_state));
    printf("Processing %zu inputs...\n", input_count);
    
    simulate_state_machine(&machine, inputs, input_count);
    
    printf("Final state: %s\n", state_name(machine.current_state));
    
    uint64_t cycle_count = 0;
    for (int i = 0; i < 1000000; i++) {
        for (size_t j = 0; j < input_count; j++) {
            machine.current_state = process_transition(&machine, inputs[j]);
            cycle_count++;
        }
    }
    
    printf("Completed %lu state transitions\n", (unsigned long)cycle_count);
    
    return 0;
}