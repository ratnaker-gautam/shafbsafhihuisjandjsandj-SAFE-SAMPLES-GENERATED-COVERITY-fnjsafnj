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
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_COUNT
} input_t;

static const state_t transition_table[STATE_COUNT][INPUT_COUNT] = {
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

int validate_input_sequence(const char* input_str, input_t* inputs, size_t max_inputs) {
    if (input_str == NULL || inputs == NULL || max_inputs == 0) {
        return -1;
    }
    
    size_t len = strlen(input_str);
    if (len == 0 || len > max_inputs) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        char c = input_str[i];
        if (c < '0' || c > '2') {
            return -1;
        }
        inputs[i] = (input_t)(c - '0');
    }
    
    return (int)len;
}

state_t process_state_machine(state_t initial_state, const input_t* inputs, int input_count) {
    state_t current_state = initial_state;
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            continue;
        }
        current_state = transition_table[current_state][inputs[i]];
    }
    
    return current_state;
}

void print_state_sequence(state_t initial_state, const input_t* inputs, int input_count) {
    state_t current_state = initial_state;
    
    printf("State sequence: %s", state_names[current_state]);
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            continue;
        }
        printf(" ->[%s]", input_names[inputs[i]]);
        current_state = transition_table[current_state][inputs[i]];
        printf("-> %s", state_names[current_state]);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[1024];
    input_t inputs[MAX_INPUTS];
    
    printf("Finite State Machine Simulation\n");
    printf("Available inputs: 0, 1, 2\n");
    printf("Enter input sequence: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input sequence\n");
        return EXIT_FAILURE;
    }
    
    int input_count = validate_input_sequence(input_buffer, inputs, MAX_INPUTS);
    if (input_count <= 0) {
        fprintf(stderr, "Invalid input sequence. Only digits 0-2 allowed.\n");
        return EXIT_FAILURE;
    }
    
    state_t initial_state = STATE_A;
    state_t final_state = process_state_machine(initial_state, inputs, input_count);
    
    printf("Initial state: %s\n", state_names[initial_state]);
    printf("Input sequence: %s\n", input_buffer);
    printf("Final state: %s\n", state_names[final_state]);
    
    print_state_sequence(initial_state, inputs, input_count);
    
    int cycle_count = 1000000;
    state_t test_state = STATE_A;
    for (int cycle = 0; cycle < cycle_count; cycle++) {
        for (int i = 0; i < input_count; i++) {
            test_state = transition_table[test_state][inputs[i]];
        }
    }
    
    printf("CPU-intensive verification completed (%d cycles)\n", cycle_count);
    
    return EXIT_SUCCESS;
}