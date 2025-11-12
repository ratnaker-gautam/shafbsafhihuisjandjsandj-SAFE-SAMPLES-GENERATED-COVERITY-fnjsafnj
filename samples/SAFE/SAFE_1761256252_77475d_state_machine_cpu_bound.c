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
    if (length == 0 || length > MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < length; i++) {
        if (inputs[i] >= INPUT_COUNT) return 0;
    }
    return 1;
}

static void simulate_state_machine(const uint8_t* inputs, size_t length) {
    State current_state = STATE_A;
    size_t transition_count = 0;
    
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < length; i++) {
        Input input = (Input)inputs[i];
        State next_state = transition_table[current_state][input];
        
        printf("Input: %u, Transition: %s -> %s\n", 
               input, state_names[current_state], state_names[next_state]);
        
        current_state = next_state;
        transition_count++;
        
        if (transition_count > 10000) {
            printf("Transition limit reached\n");
            return;
        }
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Total transitions: %zu\n", transition_count);
}

static void generate_deterministic_inputs(uint8_t* buffer, size_t length) {
    if (buffer == NULL || length == 0) return;
    
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (i * 7 + 3) % INPUT_COUNT;
    }
}

int main(void) {
    const size_t input_length = 50;
    uint8_t inputs[MAX_INPUTS];
    
    if (input_length > MAX_INPUTS) {
        fprintf(stderr, "Input length exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    generate_deterministic_inputs(inputs, input_length);
    
    if (!validate_input_sequence(inputs, input_length)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    simulate_state_machine(inputs, input_length);
    
    return EXIT_SUCCESS;
}