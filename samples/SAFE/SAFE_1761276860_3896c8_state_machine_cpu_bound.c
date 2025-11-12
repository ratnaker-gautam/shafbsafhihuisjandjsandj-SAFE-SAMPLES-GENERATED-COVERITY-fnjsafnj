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
} state_t;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[3] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

static int validate_input_sequence(const int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] > 2) {
            return 0;
        }
    }
    return 1;
}

static state_t process_state_machine(state_t initial_state, const int* inputs, size_t count) {
    state_t current_state = initial_state;
    
    for (size_t i = 0; i < count; i++) {
        input_t input = (input_t)inputs[i];
        if (current_state < 0 || current_state >= STATE_COUNT) {
            return STATE_A;
        }
        if (input < 0 || input > 2) {
            return STATE_A;
        }
        current_state = transition_table[current_state][input];
    }
    
    return current_state;
}

static void generate_deterministic_inputs(int* inputs, size_t count, unsigned int seed) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (i + seed) % 3;
    }
}

int main(void) {
    const size_t input_count = 500;
    int inputs[MAX_INPUTS];
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    generate_deterministic_inputs(inputs, input_count, 42);
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    state_t start_state = STATE_A;
    state_t final_state = process_state_machine(start_state, inputs, input_count);
    
    if (final_state < 0 || final_state >= STATE_COUNT) {
        fprintf(stderr, "Invalid final state\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state: %s\n", state_names[start_state]);
    printf("Processed %zu inputs\n", input_count);
    printf("Final state: %s\n", state_names[final_state]);
    
    size_t state_visits[STATE_COUNT] = {0};
    state_t test_state = start_state;
    
    for (size_t i = 0; i < input_count; i++) {
        if (test_state < 0 || test_state >= STATE_COUNT) {
            break;
        }
        state_visits[test_state]++;
        test_state = transition_table[test_state][inputs[i]];
    }
    
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("  %s: %zu\n", state_names[i], state_visits[i]);
    }
    
    return EXIT_SUCCESS;
}