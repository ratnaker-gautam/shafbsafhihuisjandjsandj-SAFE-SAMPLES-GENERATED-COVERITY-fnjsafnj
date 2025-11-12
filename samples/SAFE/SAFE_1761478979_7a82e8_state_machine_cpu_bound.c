//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    state_t current;
    int input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
    int computation_buffer[256];
    int buffer_size;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->buffer_size = 0;
    
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

int process_input(state_machine_t *machine, int input) {
    if (input < 0 || input >= MAX_INPUTS) return 0;
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void perform_computation(state_machine_t *machine) {
    if (machine->buffer_size >= 256) return;
    
    int base = (int)machine->current_state;
    int iterations = 1000 + (base * 100);
    
    for (int i = 0; i < iterations && machine->buffer_size < 256; i++) {
        int result = 0;
        for (int j = 0; j < 100; j++) {
            result = (result + i * j + base) % 1000;
        }
        machine->computation_buffer[machine->buffer_size] = result;
        machine->buffer_size++;
    }
}

int get_valid_input(void) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (val < 0 || val >= MAX_INPUTS) return -1;
    
    return (int)val;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("State Machine Simulation (Inputs: 0-%d, -1 to exit)\n", MAX_INPUTS - 1);
    
    int input;
    int step_count = 0;
    const int max_steps = 10000;
    
    while (step_count < max_steps) {
        printf("Current state: %d | Enter input: ", (int)machine.current_state);
        fflush(stdout);
        
        input = get_valid_input();
        if (input == -1) break;
        
        if (!process_input(&machine, input)) {
            printf("Invalid transition from state %d with input %d\n", 
                   (int)machine.current_state, input);
            continue;
        }
        
        perform_computation(&machine);
        step_count++;
        
        if (step_count % 10 == 0) {
            printf("Step %d: Buffer contains %d computation results\n", 
                   step_count, machine.buffer_size);
        }
    }
    
    int total = 0;
    for (int i = 0; i < machine.buffer_size; i++) {
        total = (total + machine.computation_buffer[i]) % 10000;
    }
    
    printf("Final state: %d | Total computation: %d | Steps: %d\n", 
           (int)machine.current_state, total, step_count);
    
    return 0;
}