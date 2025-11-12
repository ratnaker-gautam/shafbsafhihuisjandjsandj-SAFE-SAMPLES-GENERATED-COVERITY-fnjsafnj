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

int main(void) {
    Input input_sequence[MAX_INPUTS];
    size_t sequence_length = 0;
    
    printf("Enter number of inputs (1-%d): ", MAX_INPUTS);
    if (scanf("%zu", &sequence_length) != 1) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    if (sequence_length == 0 || sequence_length > MAX_INPUTS) {
        fprintf(stderr, "Input length out of bounds\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter inputs (0=X, 1=Y, 2=Z):\n");
    for (size_t i = 0; i < sequence_length; i++) {
        int input_val;
        if (scanf("%d", &input_val) != 1) {
            fprintf(stderr, "Invalid input value\n");
            return EXIT_FAILURE;
        }
        
        if (input_val < 0 || input_val > 2) {
            fprintf(stderr, "Input value out of range\n");
            return EXIT_FAILURE;
        }
        
        input_sequence[i] = (Input)input_val;
    }
    
    if (!validate_input_sequence(input_sequence, sequence_length)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    unsigned long long computation_counter = 0;
    
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < sequence_length; i++) {
        Input current_input = input_sequence[i];
        State next_state = process_transition(current_state, current_input);
        
        for (int j = 0; j < 10000; j++) {
            computation_counter += (j * j) % 100;
        }
        
        printf("Input: %s -> State: %s -> Next: %s\n", 
               input_names[current_input], 
               state_names[current_state], 
               state_names[next_state]);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Computation cycles: %llu\n", computation_counter);
    
    return EXIT_SUCCESS;
}