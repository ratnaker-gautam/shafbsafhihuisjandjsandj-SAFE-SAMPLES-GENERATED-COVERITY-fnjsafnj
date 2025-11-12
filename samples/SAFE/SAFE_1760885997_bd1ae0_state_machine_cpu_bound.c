//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_INPUTS 1000

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
    INPUT_2,
    INPUT_COUNT
} Input;

static const State transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_D, STATE_E, STATE_B},
    {STATE_F, STATE_G, STATE_C},
    {STATE_H, STATE_A, STATE_D},
    {STATE_B, STATE_C, STATE_E},
    {STATE_D, STATE_E, STATE_F},
    {STATE_F, STATE_G, STATE_G},
    {STATE_H, STATE_A, STATE_H}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E", "F", "G", "H"
};

static const char* input_names[INPUT_COUNT] = {
    "0", "1", "2"
};

int validate_input_sequence(const int* inputs, int count) {
    if (inputs == NULL || count <= 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (int i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return 0;
        }
    }
    return 1;
}

State process_state_machine(State initial, const int* inputs, int count) {
    State current = initial;
    for (int i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return current;
        }
        current = transition_table[current][inputs[i]];
    }
    return current;
}

void generate_random_inputs(int* inputs, int count) {
    for (int i = 0; i < count; i++) {
        inputs[i] = rand() % INPUT_COUNT;
    }
}

int main(void) {
    const int num_iterations = 10000;
    const int input_length = 100;
    int inputs[input_length];
    
    if (input_length <= 0 || input_length > MAX_INPUTS) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    
    State final_state = STATE_A;
    int valid_transitions = 0;
    
    for (int iter = 0; iter < num_iterations; iter++) {
        generate_random_inputs(inputs, input_length);
        
        if (!validate_input_sequence(inputs, input_length)) {
            continue;
        }
        
        State result = process_state_machine(STATE_A, inputs, input_length);
        
        if (result >= STATE_A && result < STATE_COUNT) {
            final_state = result;
            valid_transitions++;
        }
    }
    
    if (valid_transitions > 0) {
        printf("Final state after %d iterations: %s\n", 
               num_iterations, state_names[final_state]);
        printf("Valid transitions processed: %d\n", valid_transitions);
    } else {
        printf("No valid transitions processed\n");
    }
    
    return EXIT_SUCCESS;
}