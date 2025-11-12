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
    "A", "B", "C", "D", "E", "F", "G", "H"
};

static int validate_input_sequence(const uint8_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count > MAX_INPUTS) return 0;
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] >= INPUT_COUNT) return 0;
    }
    return 1;
}

static state_t process_state_machine(state_t initial, const uint8_t* inputs, size_t count) {
    state_t current = initial;
    for (size_t i = 0; i < count; i++) {
        input_t input = (input_t)inputs[i];
        current = transition_table[current][input];
    }
    return current;
}

static void generate_input_sequence(uint8_t* buffer, size_t count) {
    for (size_t i = 0; i < count; i++) {
        buffer[i] = rand() % INPUT_COUNT;
    }
}

int main(void) {
    const size_t sequence_length = 500;
    uint8_t input_sequence[MAX_INPUTS];
    
    if (sequence_length > MAX_INPUTS) {
        fprintf(stderr, "Sequence length exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    generate_input_sequence(input_sequence, sequence_length);
    
    if (!validate_input_sequence(input_sequence, sequence_length)) {
        fprintf(stderr, "Invalid input sequence generated\n");
        return EXIT_FAILURE;
    }
    
    state_t current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);
    printf("Processing %zu inputs...\n", sequence_length);
    
    for (size_t i = 0; i < sequence_length; i++) {
        input_t input = (input_t)input_sequence[i];
        state_t next_state = transition_table[current_state][input];
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    
    state_t verification_state = process_state_machine(STATE_A, input_sequence, sequence_length);
    if (current_state != verification_state) {
        fprintf(stderr, "State machine verification failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}