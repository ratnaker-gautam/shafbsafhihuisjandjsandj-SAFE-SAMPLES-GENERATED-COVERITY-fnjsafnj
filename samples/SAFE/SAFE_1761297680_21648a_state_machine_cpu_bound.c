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
} state_t;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_A, STATE_C},
    {STATE_C, STATE_B, STATE_D},
    {STATE_D, STATE_C, STATE_E},
    {STATE_E, STATE_D, STATE_A},
    {STATE_A, STATE_E, STATE_B}
};

static const char* state_names[STATE_COUNT] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static input_t parse_input(char c) {
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default: return INPUT_X;
    }
}

static void process_state_machine(const char* inputs, size_t input_len) {
    state_t current_state = STATE_A;
    size_t transition_count = 0;
    
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_len && i < MAX_INPUTS; i++) {
        input_t input = parse_input(inputs[i]);
        state_t next_state = transition_table[current_state][input];
        
        if (next_state >= STATE_A && next_state < STATE_COUNT) {
            printf("Input '%c': %s -> %s\n", 
                   inputs[i], 
                   state_names[current_state], 
                   state_names[next_state]);
            current_state = next_state;
            transition_count++;
        }
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Total transitions: %zu\n", transition_count);
}

static void generate_deterministic_input(char* buffer, size_t size) {
    const char symbols[] = "xyzXYZ";
    size_t symbol_count = strlen(symbols);
    
    for (size_t i = 0; i < size && i < MAX_INPUTS; i++) {
        buffer[i] = symbols[i % symbol_count];
    }
    buffer[size - 1] = '\0';
}

int main(void) {
    char input_sequence[MAX_INPUTS + 1];
    size_t sequence_length = 50;
    
    if (sequence_length > MAX_INPUTS || sequence_length == 0) {
        fprintf(stderr, "Invalid sequence length\n");
        return EXIT_FAILURE;
    }
    
    generate_deterministic_input(input_sequence, sequence_length + 1);
    
    printf("State Machine Simulation\n");
    printf("Input sequence: %s\n", input_sequence);
    printf("Sequence length: %zu\n", sequence_length);
    printf("---\n");
    
    process_state_machine(input_sequence, sequence_length);
    
    return EXIT_SUCCESS;
}