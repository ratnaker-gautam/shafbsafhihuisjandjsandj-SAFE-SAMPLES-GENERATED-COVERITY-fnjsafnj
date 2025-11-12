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

int validate_input_range(int value) {
    return value >= 0 && value <= 2;
}

int cpu_intensive_calculation(int base, int iterations) {
    int result = base;
    for (int i = 0; i < iterations; i++) {
        result = (result * 1103515245 + 12345) & 0x7FFFFFFF;
        result = (result >> 16) & 0x7FFF;
    }
    return result;
}

int main(void) {
    State current_state = STATE_A;
    int total_processed = 0;
    int input_count = 0;
    
    printf("Enter number of state machine inputs (1-%d): ", MAX_INPUTS);
    if (scanf("%d", &input_count) != 1) {
        fprintf(stderr, "Error reading input count\n");
        return EXIT_FAILURE;
    }
    
    if (input_count < 1 || input_count > MAX_INPUTS) {
        fprintf(stderr, "Invalid input count\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing %d state transitions...\n", input_count);
    
    for (int i = 0; i < input_count; i++) {
        int input_val = cpu_intensive_calculation(i, 1000) % 3;
        
        if (!validate_input_range(input_val)) {
            fprintf(stderr, "Generated invalid input\n");
            return EXIT_FAILURE;
        }
        
        int process_value = process_table[current_state][input_val];
        total_processed += process_value;
        
        State next_state = transition_table[current_state][input_val];
        
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        current_state = next_state;
        
        if (total_processed > INT_MAX - 1000) {
            total_processed = total_processed / 2;
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total processed value: %d\n", total_processed);
    
    return EXIT_SUCCESS;
}