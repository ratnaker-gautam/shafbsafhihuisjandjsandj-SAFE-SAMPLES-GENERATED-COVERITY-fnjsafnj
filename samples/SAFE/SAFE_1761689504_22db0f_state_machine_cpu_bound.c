//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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

static const state_t transition_table[STATE_COUNT][3] = {
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

int validate_input_sequence(const input_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count > MAX_INPUTS) return 0;
    return 1;
}

input_t generate_input(uint32_t* seed) {
    if (seed == NULL) return INPUT_X;
    *seed = (*seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (input_t)(*seed % 3);
}

int main(void) {
    const size_t input_count = 100;
    input_t inputs[MAX_INPUTS];
    uint32_t seed = 42;
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_count; i++) {
        inputs[i] = generate_input(&seed);
    }
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    state_t current_state = STATE_A;
    size_t state_visits[STATE_COUNT] = {0};
    
    printf("Starting state machine simulation\n");
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        input_t current_input = inputs[i];
        
        if (current_input < INPUT_X || current_input > INPUT_Z) {
            fprintf(stderr, "Invalid input value\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = transition_table[current_state][current_input];
        
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        printf("Step %zu: Input %s, State %s -> %s\n", 
               i + 1, input_names[current_input], 
               state_names[current_state], state_names[next_state]);
        
        current_state = next_state;
        state_visits[current_state]++;
        
        for (int j = 0; j < 1000; j++) {
            volatile int computation = j * j + j;
            (void)computation;
        }
    }
    
    printf("\nFinal state: %s\n", state_names[current_state]);
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("  %s: %zu\n", state_names[i], state_visits[i]);
    }
    
    size_t total_transitions = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        if (state_visits[i] > SIZE_MAX - total_transitions) {
            fprintf(stderr, "Integer overflow in visit count\n");
            return EXIT_FAILURE;
        }
        total_transitions += state_visits[i];
    }
    
    printf("Total state transitions: %zu\n", total_transitions);
    
    return EXIT_SUCCESS;
}