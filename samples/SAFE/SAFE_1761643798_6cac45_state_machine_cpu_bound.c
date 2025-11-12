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

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static int process_state_machine(const input_t* inputs, size_t input_count) {
    if (inputs == NULL || input_count == 0 || input_count > MAX_INPUTS) {
        return -1;
    }
    
    state_t current_state = STATE_A;
    int transition_count = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        state_t next_state = transition_table[current_state][inputs[i]];
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            return -1;
        }
        
        if (current_state != next_state) {
            transition_count++;
        }
        current_state = next_state;
    }
    
    return transition_count;
}

static int generate_random_inputs(input_t* buffer, size_t count) {
    if (buffer == NULL || count == 0 || count > MAX_INPUTS) {
        return -1;
    }
    
    for (size_t i = 0; i < count; i++) {
        int r = rand() % 3;
        buffer[i] = (input_t)r;
    }
    
    return 0;
}

int main(void) {
    const size_t input_sequence_length = 500;
    input_t inputs[MAX_INPUTS];
    
    if (generate_random_inputs(inputs, input_sequence_length) != 0) {
        fprintf(stderr, "Failed to generate inputs\n");
        return EXIT_FAILURE;
    }
    
    int result = process_state_machine(inputs, input_sequence_length);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed %d transitions\n", result);
    
    long computation_cycles = 0;
    for (int i = 0; i < 1000000; i++) {
        for (int j = 0; j < 100; j++) {
            computation_cycles += i * j;
            if (computation_cycles > LONG_MAX / 2) {
                computation_cycles = 0;
            }
        }
    }
    
    printf("Computation cycles: %ld\n", computation_cycles);
    
    return EXIT_SUCCESS;
}