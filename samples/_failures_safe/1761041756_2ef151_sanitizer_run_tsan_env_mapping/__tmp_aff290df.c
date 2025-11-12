//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

typedef struct {
    State current;
    State next;
    Input trigger;
} Transition;

static const Transition transitions[] = {
    {STATE_A, STATE_B, INPUT_X},
    {STATE_A, STATE_C, INPUT_Y},
    {STATE_B, STATE_C, INPUT_Z},
    {STATE_B, STATE_D, INPUT_X},
    {STATE_C, STATE_D, INPUT_Y},
    {STATE_C, STATE_E, INPUT_Z},
    {STATE_D, STATE_E, INPUT_X},
    {STATE_D, STATE_A, INPUT_Y},
    {STATE_E, STATE_A, INPUT_Z},
    {STATE_E, STATE_B, INPUT_X}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

State process_transition(State current, Input input) {
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void generate_inputs(Input* inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (Input)(rand() % 3);
    }
}

int main(void) {
    const size_t num_inputs = 500;
    if (num_inputs > MAX_INPUTS || num_inputs == 0) {
        fprintf(stderr, "Invalid number of inputs\n");
        return EXIT_FAILURE;
    }

    Input* inputs = malloc(num_inputs * sizeof(Input));
    if (inputs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    generate_inputs(inputs, num_inputs);

    State current_state = STATE_A;
    unsigned long long computation = 0;

    for (size_t i = 0; i < num_inputs; i++) {
        State next_state = process_transition(current_state, inputs[i]);
        
        for (int j = 0; j < 1000; j++) {
            computation += (current_state * next_state) + inputs[i] + j;
            if (computation > ULLONG_MAX / 2) {
                computation = 0;
            }
        }

        current_state = next_state;
    }

    printf("Final state: %s\n", state_names[current_state]);
    printf("Computation result: %llu\n", computation);

    free(inputs);
    return EXIT_SUCCESS;
}