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

typedef struct {
    State current;
    State next;
    Input trigger;
} Transition;

static const Transition transitions[] = {
    {STATE_A, STATE_B, INPUT_X},
    {STATE_A, STATE_C, INPUT_Y},
    {STATE_B, STATE_C, INPUT_X},
    {STATE_B, STATE_D, INPUT_Y},
    {STATE_C, STATE_D, INPUT_X},
    {STATE_C, STATE_E, INPUT_Y},
    {STATE_D, STATE_E, INPUT_X},
    {STATE_D, STATE_A, INPUT_Y},
    {STATE_E, STATE_A, INPUT_X},
    {STATE_E, STATE_B, INPUT_Y}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

State process_state_machine(State current, Input input) {
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return current;
}

int validate_input_sequence(const Input* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    return 1;
}

void simulate_state_machine(const Input* inputs, size_t count) {
    if (!validate_input_sequence(inputs, count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return;
    }

    State current = STATE_A;
    printf("Initial state: %s\n", state_names[current]);

    for (size_t i = 0; i < count; i++) {
        Input input = inputs[i];
        if (input < INPUT_X || input > INPUT_Z) {
            fprintf(stderr, "Invalid input at position %zu\n", i);
            return;
        }
        
        State next = process_state_machine(current, input);
        printf("Input: %s -> Transition: %s -> %s\n", 
               input_names[input], state_names[current], state_names[next]);
        current = next;
    }

    printf("Final state: %s\n", state_names[current]);
}

int main(void) {
    Input test_inputs[] = {
        INPUT_X, INPUT_Y, INPUT_X, INPUT_Y, INPUT_X, INPUT_Y, INPUT_X, INPUT_Y
    };
    size_t input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);

    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input sequence too long\n");
        return EXIT_FAILURE;
    }

    simulate_state_machine(test_inputs, input_count);

    int computation_cycles = 1000000;
    long result = 0;
    
    for (int i = 0; i < computation_cycles; i++) {
        State s = STATE_A;
        for (int j = 0; j < 100; j++) {
            Input in = (j % 3 == 0) ? INPUT_X : ((j % 3 == 1) ? INPUT_Y : INPUT_Z);
            s = process_state_machine(s, in);
            result = (result + s) % 1000;
        }
    }
    
    printf("Computation result: %ld\n", result);
    return EXIT_SUCCESS;
}