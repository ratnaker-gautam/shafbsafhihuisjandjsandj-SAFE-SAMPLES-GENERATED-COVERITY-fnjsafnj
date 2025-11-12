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

int validate_input_sequence(const uint8_t* inputs, size_t length) {
    if (inputs == NULL || length == 0 || length > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < length; i++) {
        if (inputs[i] >= INPUT_COUNT) {
            return 0;
        }
    }
    return 1;
}

void generate_deterministic_inputs(uint8_t* buffer, size_t length) {
    if (buffer == NULL || length == 0 || length > MAX_INPUTS) {
        return;
    }
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (i % 3 == 0) ? INPUT_0 : INPUT_1;
    }
}

int main(void) {
    const size_t input_length = 10000;
    uint8_t* inputs = malloc(input_length * sizeof(uint8_t));
    if (inputs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    generate_deterministic_inputs(inputs, input_length);
    
    if (!validate_input_sequence(inputs, input_length)) {
        fprintf(stderr, "Invalid input sequence\n");
        free(inputs);
        return EXIT_FAILURE;
    }

    State current_state = STATE_A;
    size_t state_visits[STATE_COUNT] = {0};
    
    for (size_t i = 0; i < input_length; i++) {
        if (inputs[i] >= INPUT_COUNT) {
            continue;
        }
        current_state = process_input(current_state, (Input)inputs[i]);
        if (current_state < STATE_COUNT) {
            state_visits[current_state]++;
        }
    }

    printf("Final state: %s\n", state_names[current_state]);
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("  State %s: %zu\n", state_names[i], state_visits[i]);
    }

    size_t total_transitions = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        total_transitions += state_visits[i];
    }
    printf("Total state transitions: %zu\n", total_transitions);

    free(inputs);
    return EXIT_SUCCESS;
}