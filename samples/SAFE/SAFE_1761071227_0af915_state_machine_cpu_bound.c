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

int process_state_machine(const Input* inputs, size_t input_count) {
    if (inputs == NULL || input_count == 0 || input_count > MAX_INPUTS) {
        return -1;
    }
    
    State current_state = STATE_A;
    size_t cycles = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        for (volatile int j = 0; j < 1000; j++) {
            cycles++;
        }
    }
    
    return (int)(current_state + cycles % 1000);
}

int main(void) {
    Input inputs[MAX_INPUTS];
    size_t input_count;
    
    printf("Enter number of inputs (1-%d): ", MAX_INPUTS);
    if (scanf("%zu", &input_count) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    if (input_count < 1 || input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count out of range\n");
        return 1;
    }
    
    printf("Enter inputs (0=X, 1=Y, 2=Z):\n");
    for (size_t i = 0; i < input_count; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Invalid input value\n");
            return 1;
        }
        
        if (value < 0 || value > 2) {
            fprintf(stderr, "Input value out of range\n");
            return 1;
        }
        
        inputs[i] = (Input)value;
    }
    
    int result = process_state_machine(inputs, input_count);
    if (result == -1) {
        fprintf(stderr, "State machine processing failed\n");
        return 1;
    }
    
    printf("Final result: %d\n", result);
    return 0;
}