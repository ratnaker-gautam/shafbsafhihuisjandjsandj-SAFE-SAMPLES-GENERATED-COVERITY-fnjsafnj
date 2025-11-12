//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATE_COUNT 5
#define MAX_INPUTS 1000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E
} state_t;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[3] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

static input_t generate_input(uint32_t* seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (input_t)(*seed % 3);
}

static void process_state_machine(uint32_t iterations) {
    if (iterations == 0 || iterations > MAX_INPUTS) {
        fprintf(stderr, "Error: Invalid iteration count\n");
        return;
    }

    state_t current_state = STATE_A;
    uint32_t seed = 42;
    uint32_t state_counts[STATE_COUNT] = {0};

    printf("Starting state machine with %u iterations\n", iterations);
    printf("Initial state: %s\n", state_names[current_state]);

    for (uint32_t i = 0; i < iterations; i++) {
        input_t input = generate_input(&seed);
        
        if (input < 0 || input > 2) {
            fprintf(stderr, "Error: Invalid input generated\n");
            return;
        }

        state_t next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Error: Invalid state transition\n");
            return;
        }

        printf("Step %u: Input %s -> State %s\n", 
               i + 1, input_names[input], state_names[next_state]);

        state_counts[next_state]++;
        current_state = next_state;
    }

    printf("\nFinal state: %s\n", state_names[current_state]);
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("  %s: %u\n", state_names[i], state_counts[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    char* endptr;
    long iterations = strtol(argv[1], &endptr, 10);
    
    if (*endptr != '\0' || iterations < 1 || iterations > MAX_INPUTS) {
        fprintf(stderr, "Error: Iterations must be between 1 and %d\n", MAX_INPUTS);
        return 1;
    }

    process_state_machine((uint32_t)iterations);
    return 0;
}