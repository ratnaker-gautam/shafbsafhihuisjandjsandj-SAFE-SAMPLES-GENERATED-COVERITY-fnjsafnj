//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

static transition_t transitions[MAX_TRANSITIONS] = {
    {STATE_A, 0, STATE_B}, {STATE_A, 1, STATE_C},
    {STATE_B, 0, STATE_D}, {STATE_B, 1, STATE_E},
    {STATE_C, 0, STATE_F}, {STATE_C, 1, STATE_G},
    {STATE_D, 0, STATE_H}, {STATE_D, 1, STATE_A},
    {STATE_E, 0, STATE_B}, {STATE_E, 1, STATE_C},
    {STATE_F, 0, STATE_D}, {STATE_F, 1, STATE_E},
    {STATE_G, 0, STATE_F}, {STATE_G, 1, STATE_G},
    {STATE_H, 0, STATE_H}, {STATE_H, 1, STATE_A}
};

static const char* state_names[MAX_STATES] = {
    "A", "B", "C", "D", "E", "F", "G", "H"
};

state_t get_next_state(state_t current, uint8_t input) {
    for (size_t i = 0; i < MAX_TRANSITIONS; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input == input) {
            return transitions[i].next_state;
        }
    }
    return current;
}

void simulate_state_machine(uint32_t iterations) {
    if (iterations == 0 || iterations > MAX_ITERATIONS) {
        iterations = 1000;
    }
    
    state_t current_state = STATE_A;
    uint32_t state_counts[MAX_STATES] = {0};
    
    for (uint32_t i = 0; i < iterations; i++) {
        uint8_t input = (i % MAX_INPUTS);
        state_counts[current_state]++;
        current_state = get_next_state(current_state, input);
    }
    
    printf("State machine simulation completed (%u iterations)\n", iterations);
    printf("Final state distribution:\n");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("State %s: %u visits (%.2f%%)\n", 
               state_names[i], state_counts[i], 
               (state_counts[i] * 100.0) / iterations);
    }
}

int main(int argc, char *argv[]) {
    uint32_t iterations = 10000;
    
    if (argc > 1) {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && *endptr == '\0' && val >= 0 && val <= MAX_ITERATIONS) {
            iterations = (uint32_t)val;
        } else {
            fprintf(stderr, "Invalid iteration count. Using default: %u\n", iterations);
        }
    }
    
    simulate_state_machine(iterations);
    return 0;
}