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

State process_state_machine(State start_state, const int* inputs, size_t count) {
    State current_state = start_state;
    
    for (size_t i = 0; i < count; i++) {
        if (current_state < 0 || current_state >= STATE_COUNT) {
            return STATE_A;
        }
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return STATE_A;
        }
        current_state = transition_table[current_state][inputs[i]];
    }
    
    return current_state;
}

void generate_random_inputs(int* inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = rand() % INPUT_COUNT;
    }
}

int main(void) {
    const size_t num_simulations = 100;
    const size_t input_length = 50;
    
    if (input_length > MAX_INPUTS) {
        fprintf(stderr, "Input length too large\n");
        return EXIT_FAILURE;
    }
    
    int inputs[MAX_INPUTS];
    State final_states[STATE_COUNT];
    
    for (State start_state = STATE_A; start_state < STATE_COUNT; start_state++) {
        size_t state_count = 0;
        
        for (size_t sim = 0; sim < num_simulations; sim++) {
            generate_random_inputs(inputs, input_length);
            
            if (!validate_input_sequence(inputs, input_length)) {
                fprintf(stderr, "Invalid input sequence\n");
                return EXIT_FAILURE;
            }
            
            State final_state = process_state_machine(start_state, inputs, input_length);
            
            if (final_state < 0 || final_state >= STATE_COUNT) {
                fprintf(stderr, "Invalid final state\n");
                return EXIT_FAILURE;
            }
            
            final_states[final_state]++;
            state_count++;
        }
        
        printf("Starting from state %s:\n", state_names[start_state]);
        for (State s = STATE_A; s < STATE_COUNT; s++) {
            double percentage = (double)final_states[s] / state_count * 100.0;
            printf("  Reached state %s: %.2f%%\n", state_names[s], percentage);
        }
        printf("\n");
        
        memset(final_states, 0, sizeof(final_states));
    }
    
    return EXIT_SUCCESS;
}