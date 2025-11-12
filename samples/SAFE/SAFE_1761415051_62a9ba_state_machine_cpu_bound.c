//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    INPUT_COUNT
} Input;

static const State transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C},
    {STATE_D, STATE_A},
    {STATE_E, STATE_B},
    {STATE_A, STATE_C},
    {STATE_D, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E"
};

static int validate_input_sequence(const uint8_t* inputs, size_t length) {
    if (inputs == NULL) return 0;
    if (length > MAX_INPUTS) return 0;
    for (size_t i = 0; i < length; i++) {
        if (inputs[i] >= INPUT_COUNT) return 0;
    }
    return 1;
}

static State process_state_machine(const uint8_t* inputs, size_t length) {
    State current = STATE_A;
    for (size_t i = 0; i < length; i++) {
        if (inputs[i] < INPUT_COUNT) {
            current = transition_table[current][inputs[i]];
        }
    }
    return current;
}

static void generate_random_inputs(uint8_t* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        buffer[i] = rand() % INPUT_COUNT;
    }
}

int main(void) {
    const size_t num_sequences = 100;
    const size_t sequence_length = 50;
    
    if (sequence_length > MAX_INPUTS) {
        fprintf(stderr, "Error: Sequence length exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    uint8_t inputs[MAX_INPUTS];
    State final_states[num_sequences];
    
    srand(42);
    
    for (size_t seq = 0; seq < num_sequences; seq++) {
        generate_random_inputs(inputs, sequence_length);
        
        if (!validate_input_sequence(inputs, sequence_length)) {
            fprintf(stderr, "Error: Invalid input sequence\n");
            return EXIT_FAILURE;
        }
        
        final_states[seq] = process_state_machine(inputs, sequence_length);
    }
    
    size_t state_counts[STATE_COUNT] = {0};
    for (size_t i = 0; i < num_sequences; i++) {
        if (final_states[i] < STATE_COUNT) {
            state_counts[final_states[i]]++;
        }
    }
    
    printf("State machine processing complete\n");
    printf("Processed %zu sequences of length %zu\n", num_sequences, sequence_length);
    printf("Final state distribution:\n");
    for (State s = STATE_A; s < STATE_COUNT; s++) {
        printf("State %s: %zu occurrences\n", state_names[s], state_counts[s]);
    }
    
    return EXIT_SUCCESS;
}