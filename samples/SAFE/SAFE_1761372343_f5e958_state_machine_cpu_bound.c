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
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_COUNT
} input_t;

static const state_t transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[INPUT_COUNT] = {
    "INPUT_0", "INPUT_1", "INPUT_2"
};

int validate_input_sequence(const int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return 0;
        }
    }
    return 1;
}

state_t process_state_machine(state_t initial_state, const int* inputs, size_t count) {
    state_t current_state = initial_state;
    
    for (size_t i = 0; i < count; i++) {
        input_t input = (input_t)inputs[i];
        if (input < 0 || input >= INPUT_COUNT) {
            continue;
        }
        if (current_state < 0 || current_state >= STATE_COUNT) {
            current_state = STATE_A;
        }
        current_state = transition_table[current_state][input];
    }
    
    return current_state;
}

void generate_random_inputs(int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        inputs[i] = rand() % INPUT_COUNT;
    }
}

int main(void) {
    const size_t input_count = 500;
    int inputs[MAX_INPUTS];
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    generate_random_inputs(inputs, input_count);
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    state_t current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        input_t input = (input_t)inputs[i];
        state_t next_state = transition_table[current_state][input];
        printf("Input: %s -> State: %s\n", input_names[input], state_names[next_state]);
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    
    state_t verified_state = process_state_machine(STATE_A, inputs, input_count);
    if (current_state != verified_state) {
        fprintf(stderr, "State verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed successfully\n");
    return EXIT_SUCCESS;
}