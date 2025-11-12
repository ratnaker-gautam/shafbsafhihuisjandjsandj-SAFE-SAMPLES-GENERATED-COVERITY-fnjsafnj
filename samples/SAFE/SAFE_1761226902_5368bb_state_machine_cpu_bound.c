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
    {STATE_B, STATE_D, INPUT_Z},
    {STATE_C, STATE_D, INPUT_Y},
    {STATE_C, STATE_E, INPUT_Z},
    {STATE_D, STATE_A, INPUT_X},
    {STATE_D, STATE_E, INPUT_Y},
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

int validate_input_sequence(const Input* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    return 1;
}

void generate_deterministic_inputs(Input* inputs, size_t count, unsigned seed) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (Input)((seed + i * 7919) % 3);
    }
}

int main(void) {
    const size_t input_count = 500;
    Input inputs[MAX_INPUTS];
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    generate_deterministic_inputs(inputs, input_count, 42);
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    unsigned long long transition_count = 0;
    
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        State next_state = process_transition(current_state, inputs[i]);
        
        if (next_state != current_state) {
            printf("Transition: %s -> %s via %s\n", 
                   state_names[current_state], 
                   state_names[next_state], 
                   input_names[inputs[i]]);
            transition_count++;
        }
        
        current_state = next_state;
        
        for (int j = 0; j < 1000; j++) {
            transition_count = (transition_count * 6364136223846793005ULL) + 1;
        }
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Total transitions: %llu\n", transition_count);
    
    return EXIT_SUCCESS;
}