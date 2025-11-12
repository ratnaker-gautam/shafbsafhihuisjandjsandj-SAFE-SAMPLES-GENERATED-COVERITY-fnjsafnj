//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_INPUTS 1000000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_COUNT
} Input;

State transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C},
    {STATE_D, STATE_E},
    {STATE_F, STATE_G},
    {STATE_H, STATE_A},
    {STATE_B, STATE_C},
    {STATE_D, STATE_E},
    {STATE_F, STATE_G},
    {STATE_H, STATE_A}
};

int process_state_machine(const Input* inputs, size_t input_count) {
    if (inputs == NULL || input_count == 0 || input_count > MAX_INPUTS) {
        return -1;
    }
    
    State current_state = STATE_A;
    size_t cycle_count = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= INPUT_COUNT) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        if (current_state == STATE_A) {
            cycle_count++;
        }
    }
    
    return cycle_count;
}

void generate_random_inputs(Input* buffer, size_t count) {
    if (buffer == NULL || count == 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        buffer[i] = (Input)(rand() % INPUT_COUNT);
    }
}

int main(void) {
    const size_t input_size = 100000;
    Input* inputs = malloc(input_size * sizeof(Input));
    if (inputs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    generate_random_inputs(inputs, input_size);
    
    int result = process_state_machine(inputs, input_size);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        free(inputs);
        return EXIT_FAILURE;
    }
    
    printf("State machine completed %d cycles\n", result);
    
    free(inputs);
    return EXIT_SUCCESS;
}