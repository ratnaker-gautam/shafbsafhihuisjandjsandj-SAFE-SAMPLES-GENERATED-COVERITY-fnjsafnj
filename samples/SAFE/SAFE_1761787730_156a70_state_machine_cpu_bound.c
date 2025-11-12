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
    "STATE_A", "STATE_B", "STATE_C", "STATE_D",
    "STATE_E", "STATE_F", "STATE_G", "STATE_H"
};

void generate_inputs(input_t* inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (input_t)(rand() % INPUT_COUNT);
    }
}

int validate_input_sequence(const input_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count > MAX_INPUTS) return 0;
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] >= INPUT_COUNT) return 0;
    }
    return 1;
}

void process_state_machine(const input_t* inputs, size_t count) {
    if (!validate_input_sequence(inputs, count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return;
    }

    state_t current_state = STATE_A;
    size_t transition_count = 0;

    for (size_t i = 0; i < count; i++) {
        input_t input = inputs[i];
        state_t next_state = transition_table[current_state][input];
        
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid transition detected\n");
            return;
        }

        current_state = next_state;
        transition_count++;
        
        for (int j = 0; j < 1000; j++) {
            volatile int computation = j * j + j;
            (void)computation;
        }
    }

    printf("Final state: %s\n", state_names[current_state]);
    printf("Processed %zu transitions\n", transition_count);
}

int main(void) {
    const size_t input_count = 500;
    input_t inputs[MAX_INPUTS];

    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }

    srand(42);
    generate_inputs(inputs, input_count);

    process_state_machine(inputs, input_count);

    return EXIT_SUCCESS;
}