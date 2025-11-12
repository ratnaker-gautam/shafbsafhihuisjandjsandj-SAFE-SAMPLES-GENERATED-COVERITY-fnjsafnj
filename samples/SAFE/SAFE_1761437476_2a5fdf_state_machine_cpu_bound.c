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
    if (count > MAX_TRANSITIONS) {
        count = MAX_TRANSITIONS;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = trans[i];
            machine->transition_count++;
        }
    }
}

static state_t process_transition(const state_machine_t *machine, uint8_t input) {
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current_state == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next_state;
        }
    }
    return machine->current_state;
}

static uint8_t generate_input(uint64_t iteration) {
    return (uint8_t)((iteration * 1103515245 + 12345) % MAX_INPUTS);
}

static void print_state_name(state_t state) {
    const char *names[] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    if (state < MAX_STATES) {
        printf("%s", names[state]);
    } else {
        printf("?");
    }
}

int main(void) {
    state_machine_t machine;
    uint64_t iterations = 0;
    uint64_t max_iterations = MAX_ITERATIONS;
    
    initialize_machine(&machine);
    
    printf("State Machine Simulation\n");
    printf("Starting state: ");
    print_state_name(machine.current_state);
    printf("\n");
    
    uint64_t state_counts[MAX_STATES] = {0};
    
    for (uint64_t i = 0; i < max_iterations; i++) {
        if (i >= max_iterations) {
            break;
        }
        
        uint8_t input = generate_input(i);
        if (input >= MAX_INPUTS) {
            input = 0;
        }
        
        state_t new_state = process_transition(&machine, input);
        if (new_state < MAX_STATES) {
            state_counts[new_state]++;
        }
        
        machine.current_state = new_state;
        iterations++;
        
        if (iterations % 200000 == 0) {
            printf("Iteration %lu: State ", iterations);
            print_state_name(machine.current_state);
            printf(", Input %u\n", input);
        }
    }
    
    printf("\nFinal state distribution:\n");
    for (state_t s = 0; s < MAX_STATES; s++) {
        printf("State ");
        print_state_name(s);
        printf(": %lu visits\n", state_counts[s]);
    }
    
    printf("Total iterations: %lu\n", iterations);
    
    return 0;
}