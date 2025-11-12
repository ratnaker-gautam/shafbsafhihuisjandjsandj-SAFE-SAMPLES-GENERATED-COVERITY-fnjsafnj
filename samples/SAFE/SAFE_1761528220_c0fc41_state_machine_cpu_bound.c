//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
} state_t;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_COUNT
} input_t;

static const state_t transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C},
    {STATE_D, STATE_A},
    {STATE_E, STATE_F},
    {STATE_G, STATE_H},
    {STATE_A, STATE_B},
    {STATE_C, STATE_D},
    {STATE_E, STATE_F},
    {STATE_G, STATE_H}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E", "F", "G", "H"
};

static int validate_input_sequence(const uint8_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count > MAX_INPUTS) return 0;
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] >= INPUT_COUNT) return 0;
    }
    return 1;
}

static state_t process_state_machine(state_t initial, const uint8_t* inputs, size_t count) {
    state_t current = initial;
    for (size_t i = 0; i < count; i++) {
        input_t input = (input_t)inputs[i];
        current = transition_table[current][input];
    }
    return current;
}

static void generate_input_sequence(uint8_t* buffer, size_t count) {
    for (size_t i = 0; i < count; i++) {
        buffer[i] = rand() % INPUT_COUNT;
    }
}

static int run_state_machine_simulation(void) {
    const size_t num_simulations = 100;
    const size_t input_length = 50;
    
    if (input_length > MAX_INPUTS) {
        return EXIT_FAILURE;
    }
    
    uint8_t inputs[MAX_INPUTS];
    
    for (int sim = 0; sim < num_simulations; sim++) {
        generate_input_sequence(inputs, input_length);
        
        if (!validate_input_sequence(inputs, input_length)) {
            return EXIT_FAILURE;
        }
        
        state_t final_state = process_state_machine(STATE_A, inputs, input_length);
        
        printf("Simulation %d: Final state = %s\n", sim + 1, state_names[final_state]);
        
        for (int i = 0; i < 1000; i++) {
            state_t temp = process_state_machine(final_state, inputs, 10);
            final_state = temp;
        }
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    srand(42);
    
    int result = run_state_machine_simulation();
    if (result != EXIT_SUCCESS) {
        fprintf(stderr, "State machine simulation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine simulation completed successfully\n");
    return EXIT_SUCCESS;
}