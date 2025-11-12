//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

int process_state_machine(Input* inputs, int input_count) {
    if (inputs == NULL || input_count <= 0 || input_count > MAX_INPUTS) {
        return -1;
    }
    
    State current_state = STATE_A;
    int cycle_count = 0;
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        cycle_count++;
        
        for (int j = 0; j < 10000; j++) {
            cycle_count = (cycle_count + j) % 1000;
        }
    }
    
    return cycle_count;
}

void generate_random_inputs(Input* buffer, int count) {
    if (buffer == NULL || count <= 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        buffer[i] = (Input)(rand() % 3);
    }
}

int main(void) {
    const int test_count = 5;
    const int input_size = 500;
    
    Input test_inputs[MAX_INPUTS];
    
    for (int test = 0; test < test_count; test++) {
        generate_random_inputs(test_inputs, input_size);
        
        int result = process_state_machine(test_inputs, input_size);
        if (result < 0) {
            fprintf(stderr, "Error processing state machine\n");
            return 1;
        }
        
        printf("Test %d: Processed %d inputs with %d cycles\n", 
               test + 1, input_size, result);
    }
    
    Input edge_case[1] = {INPUT_X};
    int edge_result = process_state_machine(edge_case, 1);
    if (edge_result >= 0) {
        printf("Edge case: Single input processed with %d cycles\n", edge_result);
    }
    
    int null_result = process_state_machine(NULL, 10);
    if (null_result == -1) {
        printf("Null input correctly rejected\n");
    }
    
    Input invalid_inputs[3] = {INPUT_X, (Input)5, INPUT_Y};
    int invalid_result = process_state_machine(invalid_inputs, 3);
    if (invalid_result == -1) {
        printf("Invalid input correctly rejected\n");
    }
    
    return 0;
}