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
    INPUT_2,
    INPUT_COUNT
} Input;

static const State transition_table[STATE_COUNT][INPUT_COUNT] = {
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

State process_state_machine(State initial, const int* inputs, size_t count) {
    State current = initial;
    for (size_t i = 0; i < count; i++) {
        Input input = (Input)inputs[i];
        current = transition_table[current][input];
    }
    return current;
}

void generate_deterministic_inputs(int* inputs, size_t count, unsigned seed) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (i + seed) % INPUT_COUNT;
    }
}

int main(void) {
    const size_t input_count = 500;
    int inputs[MAX_INPUTS];
    
    generate_deterministic_inputs(inputs, input_count, 42);
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Input validation failed\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        Input input = (Input)inputs[i];
        State next_state = transition_table[current_state][input];
        printf("Input: %s -> Transition: %s -> %s\n", 
               input_names[input], state_names[current_state], state_names[next_state]);
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    
    State verification_state = process_state_machine(STATE_A, inputs, input_count);
    if (current_state != verification_state) {
        fprintf(stderr, "State machine verification failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}