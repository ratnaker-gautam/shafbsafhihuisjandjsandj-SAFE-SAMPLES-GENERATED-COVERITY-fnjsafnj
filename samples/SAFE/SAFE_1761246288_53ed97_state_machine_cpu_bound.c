//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

static const state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[3] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

static int validate_input_sequence(const input_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count > MAX_INPUTS) return 0;
    return 1;
}

static input_t generate_input(size_t iteration) {
    return (input_t)(iteration % 3);
}

static void process_state_machine(state_t initial_state, const input_t* inputs, size_t input_count) {
    state_t current_state = initial_state;
    
    for (size_t i = 0; i < input_count; i++) {
        input_t current_input = inputs[i];
        
        if (current_input < INPUT_X || current_input > INPUT_Z) {
            fprintf(stderr, "Invalid input detected\n");
            return;
        }
        
        state_t next_state = transition_table[current_state][current_input];
        
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return;
        }
        
        printf("Step %zu: State %s + Input %s -> State %s\n",
               i + 1,
               state_names[current_state],
               input_names[current_input],
               state_names[next_state]);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
}

int main(void) {
    const size_t sequence_length = 50;
    
    if (sequence_length > MAX_INPUTS) {
        fprintf(stderr, "Sequence length exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    input_t* input_sequence = malloc(sequence_length * sizeof(input_t));
    if (input_sequence == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < sequence_length; i++) {
        input_sequence[i] = generate_input(i);
    }
    
    if (!validate_input_sequence(input_sequence, sequence_length)) {
        fprintf(stderr, "Input sequence validation failed\n");
        free(input_sequence);
        return EXIT_FAILURE;
    }
    
    process_state_machine(STATE_A, input_sequence, sequence_length);
    
    free(input_sequence);
    return EXIT_SUCCESS;
}