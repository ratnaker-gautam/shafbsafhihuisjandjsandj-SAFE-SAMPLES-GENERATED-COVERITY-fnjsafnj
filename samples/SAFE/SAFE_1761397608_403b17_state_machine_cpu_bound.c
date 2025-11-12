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

static int validate_input_sequence(const input_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count == 0 || count > MAX_INPUTS) return 0;
    return 1;
}

static input_t generate_input(size_t index) {
    const uint32_t primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    uint32_t val = (index * primes[index % 10]) % 100;
    if (val < 33) return INPUT_X;
    if (val < 66) return INPUT_Y;
    return INPUT_Z;
}

static void process_state_machine(state_t initial_state, const input_t* inputs, size_t input_count) {
    state_t current_state = initial_state;
    
    for (size_t i = 0; i < input_count; i++) {
        if (current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state encountered\n");
            return;
        }
        
        input_t current_input = inputs[i];
        if (current_input > INPUT_Z) {
            fprintf(stderr, "Invalid input encountered\n");
            return;
        }
        
        printf("Step %zu: State %s, Input %s -> ", 
               i + 1, state_names[current_state], input_names[current_input]);
        
        state_t next_state = transition_table[current_state][current_input];
        
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid transition\n");
            return;
        }
        
        printf("State %s\n", state_names[next_state]);
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
}

int main(void) {
    const size_t input_count = 50;
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    input_t inputs[MAX_INPUTS];
    
    for (size_t i = 0; i < input_count; i++) {
        inputs[i] = generate_input(i);
    }
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    printf("State Machine Simulation (%zu steps)\n", input_count);
    printf("Initial state: %s\n", state_names[STATE_A]);
    
    process_state_machine(STATE_A, inputs, input_count);
    
    return EXIT_SUCCESS;
}