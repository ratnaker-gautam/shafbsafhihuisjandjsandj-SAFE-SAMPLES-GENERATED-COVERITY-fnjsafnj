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

void generate_input_sequence(Input* inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (Input)(rand() % 3);
    }
}

int main(void) {
    const size_t sequence_length = 50;
    Input input_sequence[MAX_INPUTS];
    
    if (sequence_length > MAX_INPUTS) {
        fprintf(stderr, "Sequence length exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    generate_input_sequence(input_sequence, sequence_length);
    
    if (!validate_input_sequence(input_sequence, sequence_length)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    unsigned long long computation_counter = 0;
    
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < sequence_length; i++) {
        Input current_input = input_sequence[i];
        
        if (current_input < INPUT_X || current_input > INPUT_Z) {
            fprintf(stderr, "Invalid input value\n");
            return EXIT_FAILURE;
        }
        
        printf("Input: %s -> ", input_names[current_input]);
        
        State next_state = process_state_machine(current_state, current_input);
        
        for (int j = 0; j < 1000; j++) {
            computation_counter += (current_state * 31 + next_state * 17 + current_input * 13);
            computation_counter = computation_counter % 1000000007;
        }
        
        current_state = next_state;
        printf("New state: %s\n", state_names[current_state]);
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Computation counter: %llu\n", computation_counter);
    
    return EXIT_SUCCESS;
}