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
    INPUT_0,
    INPUT_1,
    INPUT_INVALID
} Input;

State transition_table[STATE_COUNT][2] = {
    {STATE_B, STATE_C},
    {STATE_C, STATE_D},
    {STATE_D, STATE_E},
    {STATE_E, STATE_A},
    {STATE_A, STATE_B}
};

Input validate_input(int value) {
    if (value == 0) return INPUT_0;
    if (value == 1) return INPUT_1;
    return INPUT_INVALID;
}

void generate_inputs(int *inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = rand() % 2;
    }
}

int main(void) {
    int inputs[MAX_INPUTS];
    size_t input_count = 500;
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    generate_inputs(inputs, input_count);
    
    State current_state = STATE_A;
    size_t state_visits[STATE_COUNT] = {0};
    
    for (size_t i = 0; i < input_count; i++) {
        Input input = validate_input(inputs[i]);
        if (input == INPUT_INVALID) {
            fprintf(stderr, "Invalid input encountered\n");
            return EXIT_FAILURE;
        }
        
        if (current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state encountered\n");
            return EXIT_FAILURE;
        }
        
        State next_state = transition_table[current_state][input];
        
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid transition encountered\n");
            return EXIT_FAILURE;
        }
        
        state_visits[current_state]++;
        current_state = next_state;
    }
    
    state_visits[current_state]++;
    
    printf("Final state: %d\n", current_state);
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("State %d: %zu\n", i, state_visits[i]);
    }
    
    size_t total_transitions = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        total_transitions += state_visits[i];
    }
    printf("Total state visits: %zu\n", total_transitions);
    
    return EXIT_SUCCESS;
}