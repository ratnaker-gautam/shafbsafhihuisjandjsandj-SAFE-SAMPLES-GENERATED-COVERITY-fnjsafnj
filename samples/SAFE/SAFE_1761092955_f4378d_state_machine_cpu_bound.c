//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static int process_state_machine(const input_t* inputs, size_t input_count) {
    state_t current_state = STATE_A;
    size_t cycle_count = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        if (current_state == STATE_E) {
            cycle_count++;
        }
    }
    
    return cycle_count;
}

static int generate_inputs(input_t* buffer, size_t count) {
    if (count == 0 || count > MAX_INPUTS) {
        return -1;
    }
    
    for (size_t i = 0; i < count; i++) {
        buffer[i] = (input_t)(i % 3);
    }
    
    return 0;
}

int main(void) {
    input_t inputs[MAX_INPUTS];
    size_t input_count = 500;
    
    if (generate_inputs(inputs, input_count) != 0) {
        fprintf(stderr, "Failed to generate inputs\n");
        return EXIT_FAILURE;
    }
    
    int result = process_state_machine(inputs, input_count);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed %d cycles\n", result);
    
    return EXIT_SUCCESS;
}