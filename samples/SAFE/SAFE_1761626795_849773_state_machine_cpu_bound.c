//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    INPUT_0,
    INPUT_1,
    INPUT_2
} input_t;

static const state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static int process_state_machine(const input_t *inputs, size_t input_count) {
    state_t current_state = STATE_A;
    int cycle_count = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] > INPUT_2) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        cycle_count++;
        
        for (int j = 0; j < 1000; j++) {
            cycle_count = (cycle_count * 3 + 1) % 1000000;
        }
    }
    
    return cycle_count;
}

static int validate_and_generate_inputs(input_t *inputs, size_t count) {
    if (count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (input_t)((i * 7 + 3) % 3);
    }
    
    return 1;
}

int main(void) {
    input_t inputs[MAX_INPUTS];
    size_t input_count = 500;
    
    if (!validate_and_generate_inputs(inputs, input_count)) {
        fprintf(stderr, "Input validation failed\n");
        return EXIT_FAILURE;
    }
    
    int result = process_state_machine(inputs, input_count);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed with result: %d\n", result);
    return EXIT_SUCCESS;
}