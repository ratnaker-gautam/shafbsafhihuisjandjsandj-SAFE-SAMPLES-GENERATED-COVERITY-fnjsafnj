//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
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

static int validate_input_sequence(const input_t* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return 0;
        }
    }
    return 1;
}

static void process_state_machine(const input_t* inputs, size_t count) {
    state_t current_state = STATE_A;
    size_t transition_count = 0;
    
    for (size_t i = 0; i < count; i++) {
        input_t current_input = inputs[i];
        state_t next_state = transition_table[current_state][current_input];
        
        printf("Step %zu: %s + %s -> %s\n", 
               transition_count,
               state_names[current_state],
               input_names[current_input],
               state_names[next_state]);
        
        current_state = next_state;
        transition_count++;
        
        for (int j = 0; j < 1000; j++) {
            volatile int computation = j * j + j;
            (void)computation;
        }
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Total transitions: %zu\n", transition_count);
}

int main(void) {
    input_t test_inputs[] = {
        INPUT_X, INPUT_Y, INPUT_Z, INPUT_X, INPUT_Y,
        INPUT_Z, INPUT_X, INPUT_Y, INPUT_Z, INPUT_X
    };
    
    size_t input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    if (!validate_input_sequence(test_inputs, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return 1;
    }
    
    printf("Starting state machine simulation\n");
    printf("Input sequence length: %zu\n", input_count);
    
    process_state_machine(test_inputs, input_count);
    
    return 0;
}