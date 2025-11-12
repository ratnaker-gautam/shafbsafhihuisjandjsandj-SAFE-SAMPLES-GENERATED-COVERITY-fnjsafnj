//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    {1, 2, 0},
    {3, 4, 1},
    {5, 6, 2},
    {7, 8, 3},
    {9, 10, 4}
};

int validate_input(const char* input_str, Input* input_val) {
    if (input_str == NULL) return 0;
    if (strlen(input_str) != 1) return 0;
    
    switch (input_str[0]) {
        case 'x': case 'X': *input_val = INPUT_X; return 1;
        case 'y': case 'Y': *input_val = INPUT_Y; return 1;
        case 'z': case 'Z': *input_val = INPUT_Z; return 1;
        default: return 0;
    }
}

int read_inputs(Input* inputs, int max_inputs) {
    if (inputs == NULL || max_inputs <= 0) return 0;
    
    char buffer[16];
    int count = 0;
    
    printf("Enter state machine inputs (x/y/z, one per line, empty to finish):\n");
    
    while (count < max_inputs) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        Input input_val;
        if (validate_input(buffer, &input_val)) {
            inputs[count] = input_val;
            count++;
        } else {
            printf("Invalid input '%s'. Use x, y, or z.\n", buffer);
        }
    }
    
    return count;
}

int perform_computation(int base, int iterations) {
    if (iterations <= 0 || base <= 0) return 0;
    
    int result = base;
    for (int i = 0; i < iterations; i++) {
        result = (result * 3 + 7) % 1000;
        if (result < 0) result = -result;
    }
    return result;
}

void run_state_machine(Input* inputs, int input_count) {
    if (inputs == NULL || input_count <= 0) return;
    
    State current_state = STATE_A;
    int total_result = 0;
    
    printf("Starting state machine execution...\n");
    
    for (int i = 0; i < input_count; i++) {
        Input current_input = inputs[i];
        
        if (current_input < 0 || current_input > 2) continue;
        if (current_state < 0 || current_state >= STATE_COUNT) break;
        
        int computation = perform_computation(process_table[current_state][current_input], 1000);
        total_result = (total_result + computation) % 10000;
        
        printf("State: %d, Input: %d, Computation: %d, Total: %d\n", 
               current_state, current_input, computation, total_result);
        
        State next_state = transition_table[current_state][current_input];
        if (next_state < 0 || next_state >= STATE_COUNT) break;
        current_state = next_state;
    }
    
    printf("Final state: %d, Final result: %d\n", current_state, total_result);
}

int main(void) {
    Input inputs[MAX_INPUTS];
    
    int input_count = read_inputs(inputs, MAX_INPUTS);
    if (input_count <= 0) {
        printf("No valid inputs provided.\n");
        return 1;
    }
    
    if (input_count > MAX_INPUTS) {
        printf("Too many inputs.\n");
        return 1;
    }
    
    run_state_machine(inputs, input_count);
    
    return 0;
}