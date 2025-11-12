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
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_COUNT
} Input;

static const State transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E"
};

static const char* input_names[INPUT_COUNT] = {
    "0", "1", "2"
};

int validate_input_sequence(const int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return 0;
        }
    }
    return 1;
}

State process_state_machine(State initial, const int* inputs, size_t count) {
    State current = initial;
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return current;
        }
        current = transition_table[current][inputs[i]];
    }
    return current;
}

void generate_random_inputs(int* inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = rand() % INPUT_COUNT;
    }
}

int main(void) {
    const size_t input_count = 500;
    int inputs[MAX_INPUTS];
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    srand(42);
    generate_random_inputs(inputs, input_count);
    
    if (!validate_input_sequence(inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            fprintf(stderr, "Invalid input at position %zu\n", i);
            return EXIT_FAILURE;
        }
        State previous = current_state;
        current_state = transition_table[current_state][inputs[i]];
        printf("Input: %s | State: %s -> %s\n", 
               input_names[inputs[i]], 
               state_names[previous], 
               state_names[current_state]);
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    
    State verification = process_state_machine(STATE_A, inputs, input_count);
    if (verification != current_state) {
        fprintf(stderr, "State verification failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}