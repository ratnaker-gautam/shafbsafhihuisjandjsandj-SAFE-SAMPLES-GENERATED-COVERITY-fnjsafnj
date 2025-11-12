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
} State;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} Input;

State transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

int process_table[STATE_COUNT][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {10, 11, 12},
    {13, 14, 15}
};

int validate_input_sequence(const int* inputs, int count) {
    if (inputs == NULL || count <= 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (int i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] > 2) {
            return 0;
        }
    }
    return 1;
}

int cpu_intensive_work(int value, int iterations) {
    if (iterations <= 0 || iterations > 1000000) {
        return 0;
    }
    
    int result = value;
    for (int i = 0; i < iterations; i++) {
        result = (result * 1103515245 + 12345) & 0x7FFFFFFF;
        result = (result >> 16) & 0x7FFF;
        result = result ^ (result << 13);
        result = result & 0x7FFFFFFF;
    }
    return result;
}

int main(void) {
    int input_sequence[] = {INPUT_X, INPUT_Y, INPUT_Z, INPUT_X, INPUT_Y, INPUT_Z, INPUT_X, INPUT_Y};
    int sequence_count = sizeof(input_sequence) / sizeof(input_sequence[0]);
    
    if (!validate_input_sequence(input_sequence, sequence_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    int total_result = 0;
    int iteration_count = 10000;
    
    if (iteration_count <= 0 || iteration_count > 1000000) {
        fprintf(stderr, "Invalid iteration count\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < sequence_count; i++) {
        Input current_input = input_sequence[i];
        
        if (current_input < 0 || current_input > 2) {
            fprintf(stderr, "Invalid input value\n");
            return EXIT_FAILURE;
        }
        
        if (current_state < 0 || current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state\n");
            return EXIT_FAILURE;
        }
        
        int process_value = process_table[current_state][current_input];
        int computed = cpu_intensive_work(process_value, iteration_count);
        
        if (total_result > INT_MAX - computed) {
            fprintf(stderr, "Integer overflow detected\n");
            return EXIT_FAILURE;
        }
        
        total_result += computed;
        current_state = transition_table[current_state][current_input];
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total computation result: %d\n", total_result);
    
    return EXIT_SUCCESS;
}