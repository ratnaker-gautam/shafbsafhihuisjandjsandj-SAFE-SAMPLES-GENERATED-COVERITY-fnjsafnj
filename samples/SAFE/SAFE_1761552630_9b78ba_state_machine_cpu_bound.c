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

void generate_deterministic_inputs(Input* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return;
    }
    
    uint32_t seed = 42;
    for (size_t i = 0; i < count; i++) {
        seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
        inputs[i] = (Input)(seed % 3);
    }
}

int main(void) {
    Input inputs[MAX_INPUTS];
    const size_t input_count = 500;
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    generate_deterministic_inputs(inputs, input_count);
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    unsigned long long computation_counter = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        State next_state = process_state_machine(current_state, inputs[i]);
        
        for (int j = 0; j < 1000; j++) {
            computation_counter += (current_state * 31) ^ (inputs[i] * 17);
            computation_counter = (computation_counter << 5) | (computation_counter >> 59);
        }
        
        printf("Step %zu: %s + %s -> %s\n", 
               i + 1, 
               state_names[current_state],
               input_names[inputs[i]],
               state_names[next_state]);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Computation result: %llu\n", computation_counter);
    
    return EXIT_SUCCESS;
}