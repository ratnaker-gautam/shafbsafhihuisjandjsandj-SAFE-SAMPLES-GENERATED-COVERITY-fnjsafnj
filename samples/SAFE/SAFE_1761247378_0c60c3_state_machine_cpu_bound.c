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

static const State transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C},
    {STATE_D, STATE_E},
    {STATE_F, STATE_G},
    {STATE_H, STATE_A},
    {STATE_B, STATE_C},
    {STATE_D, STATE_E},
    {STATE_F, STATE_G},
    {STATE_H, STATE_A}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E", "F", "G", "H"
};

State process_input(State current, Input input) {
    if (current >= STATE_COUNT || input >= INPUT_COUNT) {
        return STATE_A;
    }
    return transition_table[current][input];
}

void generate_deterministic_inputs(int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (i * 17 + 13) % INPUT_COUNT;
    }
}

int main(void) {
    const size_t num_inputs = 100000;
    int* inputs = malloc(num_inputs * sizeof(int));
    if (inputs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    generate_deterministic_inputs(inputs, num_inputs);
    
    State current_state = STATE_A;
    size_t state_visits[STATE_COUNT] = {0};
    
    for (size_t i = 0; i < num_inputs; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            continue;
        }
        
        current_state = process_input(current_state, (Input)inputs[i]);
        if (current_state < STATE_COUNT) {
            state_visits[current_state]++;
        }
    }
    
    printf("State machine simulation completed\n");
    printf("Final state: %s\n", state_names[current_state]);
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("  State %s: %zu\n", state_names[i], state_visits[i]);
    }
    
    free(inputs);
    return EXIT_SUCCESS;
}