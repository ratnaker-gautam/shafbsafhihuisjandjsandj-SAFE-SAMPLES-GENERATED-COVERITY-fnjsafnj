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

int validate_input_range(int value, int min, int max) {
    if (value < min || value > max) {
        return 0;
    }
    return 1;
}

Input get_next_input(int iteration) {
    return (Input)((iteration * 7 + 13) % 3);
}

int perform_computation(State current_state, Input input) {
    int base = process_table[current_state][input];
    int result = 0;
    
    for (int i = 0; i < 1000; i++) {
        result = (result + base * i) % 1000000;
        if (result < 0) {
            result += 1000000;
        }
    }
    
    return result;
}

int main(void) {
    int num_iterations;
    
    printf("Enter number of state machine iterations (1-1000): ");
    if (scanf("%d", &num_iterations) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input_range(num_iterations, 1, MAX_INPUTS)) {
        fprintf(stderr, "Error: Iterations must be between 1 and 1000\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    int total_result = 0;
    
    printf("Starting state machine simulation...\n");
    
    for (int i = 0; i < num_iterations; i++) {
        Input input = get_next_input(i);
        
        if (input < 0 || input > 2) {
            fprintf(stderr, "Error: Invalid input generated\n");
            return EXIT_FAILURE;
        }
        
        int computation_result = perform_computation(current_state, input);
        
        if (computation_result < 0) {
            fprintf(stderr, "Error: Computation result out of range\n");
            return EXIT_FAILURE;
        }
        
        total_result = (total_result + computation_result) % 1000000000;
        if (total_result < 0) {
            total_result += 1000000000;
        }
        
        State next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Error: Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        printf("Iteration %d: State %d, Input %d, Result %d -> State %d\n",
               i + 1, current_state, input, computation_result, next_state);
        
        current_state = next_state;
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total computation result: %d\n", total_result);
    
    return EXIT_SUCCESS;
}