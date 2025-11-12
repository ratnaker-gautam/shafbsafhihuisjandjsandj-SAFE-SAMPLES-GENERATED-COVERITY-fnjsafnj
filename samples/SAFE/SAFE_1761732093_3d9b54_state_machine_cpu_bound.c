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

int process_inputs(Input* inputs, int count) {
    if (inputs == NULL || count <= 0 || count > MAX_INPUTS) {
        return -1;
    }
    
    State current_state = STATE_A;
    int transitions = 0;
    
    for (int i = 0; i < count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        State next_state = transition_table[current_state][inputs[i]];
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            return -1;
        }
        
        current_state = next_state;
        transitions++;
        
        if (current_state == STATE_E) {
            break;
        }
    }
    
    return transitions;
}

int generate_inputs(Input* buffer, int count) {
    if (buffer == NULL || count <= 0 || count > MAX_INPUTS) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        buffer[i] = (Input)(i % 3);
    }
    
    return 0;
}

int main(void) {
    const int input_count = 500;
    Input inputs[MAX_INPUTS];
    
    if (generate_inputs(inputs, input_count) != 0) {
        fprintf(stderr, "Failed to generate inputs\n");
        return EXIT_FAILURE;
    }
    
    int result = process_inputs(inputs, input_count);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed %d transitions\n", result);
    
    State test_state = STATE_A;
    int cycle_count = 0;
    const int max_cycles = 10000;
    
    while (cycle_count < max_cycles) {
        for (int i = 0; i < 3; i++) {
            State next_state = transition_table[test_state][i];
            if (next_state < STATE_A || next_state >= STATE_COUNT) {
                fprintf(stderr, "Invalid state transition\n");
                return EXIT_FAILURE;
            }
            test_state = next_state;
        }
        cycle_count++;
        
        if (test_state == STATE_A && cycle_count % 100 == 0) {
            break;
        }
    }
    
    printf("Completed %d state machine cycles\n", cycle_count);
    
    return EXIT_SUCCESS;
}