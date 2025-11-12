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

static state_t transition_table[STATE_COUNT][3] = {
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

static int validate_input_sequence(const int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] > 2) {
            return 0;
        }
    }
    return 1;
}

static state_t process_state_machine(state_t start_state, const int* inputs, size_t count) {
    state_t current_state = start_state;
    for (size_t i = 0; i < count; i++) {
        input_t input = (input_t)inputs[i];
        current_state = transition_table[current_state][input];
    }
    return current_state;
}

static void generate_input_sequence(int* inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = rand() % 3;
    }
}

int main(void) {
    const size_t sequence_length = 50;
    int input_sequence[MAX_INPUTS];
    
    if (sequence_length > MAX_INPUTS) {
        fprintf(stderr, "Error: Sequence length exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    generate_input_sequence(input_sequence, sequence_length);
    
    if (!validate_input_sequence(input_sequence, sequence_length)) {
        fprintf(stderr, "Error: Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_names[STATE_A]);
    printf("Input sequence length: %zu\n", sequence_length);
    
    printf("Input sequence: ");
    for (size_t i = 0; i < sequence_length; i++) {
        printf("%s ", input_names[input_sequence[i]]);
    }
    printf("\n");
    
    state_t final_state = process_state_machine(STATE_A, input_sequence, sequence_length);
    
    printf("Final state: %s\n", state_names[final_state]);
    
    int state_visits[STATE_COUNT] = {0};
    state_t current_state = STATE_A;
    state_visits[current_state]++;
    
    for (size_t i = 0; i < sequence_length; i++) {
        current_state = transition_table[current_state][input_sequence[i]];
        if (current_state < 0 || current_state >= STATE_COUNT) {
            fprintf(stderr, "Error: Invalid state transition\n");
            return EXIT_FAILURE;
        }
        state_visits[current_state]++;
    }
    
    printf("State visit counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("  %s: %d\n", state_names[i], state_visits[i]);
    }
    
    return EXIT_SUCCESS;
}