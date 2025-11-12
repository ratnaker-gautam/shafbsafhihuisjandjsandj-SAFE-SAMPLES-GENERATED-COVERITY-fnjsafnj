//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

static int process_count = 0;

static input_t generate_input(state_t current_state) {
    uint32_t seed = (uint32_t)current_state * 7919 + process_count;
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (input_t)(seed % 3);
}

static void perform_computation(state_t state) {
    volatile uint64_t result = 0;
    uint32_t iterations = 10000 + (state * 500);
    
    for (uint32_t i = 0; i < iterations; i++) {
        result += (i * i) % 1000;
        if (result > UINT64_MAX / 2) {
            result = result / 3;
        }
    }
    
    process_count++;
}

int main(void) {
    state_t current_state = STATE_A;
    uint32_t step_count = 0;
    uint32_t max_steps = 100;
    
    printf("Starting state machine simulation\n");
    
    while (step_count < max_steps) {
        if (current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state encountered\n");
            return 1;
        }
        
        input_t input = generate_input(current_state);
        if (input >= 3) {
            fprintf(stderr, "Invalid input generated\n");
            return 1;
        }
        
        printf("Step %u: State %d, Input %d\n", step_count, current_state, input);
        
        perform_computation(current_state);
        
        state_t next_state = transition_table[current_state][input];
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid transition detected\n");
            return 1;
        }
        
        current_state = next_state;
        step_count++;
        
        if (process_count > MAX_INPUTS) {
            fprintf(stderr, "Process count exceeded maximum\n");
            return 1;
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total computations: %d\n", process_count);
    
    return 0;
}