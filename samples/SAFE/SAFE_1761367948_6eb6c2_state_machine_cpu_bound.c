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
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E"
};

static input_t parse_input(char c) {
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default: return INPUT_X;
    }
}

static void process_state_machine(const char* input_sequence) {
    state_t current_state = STATE_A;
    size_t input_len = strlen(input_sequence);
    
    if (input_len == 0 || input_len > MAX_INPUTS) {
        printf("Invalid input sequence length\n");
        return;
    }
    
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_len; i++) {
        input_t input = parse_input(input_sequence[i]);
        state_t next_state = transition_table[current_state][input];
        
        printf("Input: %c -> State transition: %s -> %s\n", 
               input_sequence[i], state_names[current_state], state_names[next_state]);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
}

static void generate_test_input(char* buffer, size_t size) {
    const char symbols[] = "xyzXYZ";
    size_t symbols_len = strlen(symbols);
    
    if (size < 2) return;
    
    for (size_t i = 0; i < size - 1; i++) {
        buffer[i] = symbols[rand() % symbols_len];
    }
    buffer[size - 1] = '\0';
}

int main(void) {
    char input_buffer[MAX_INPUTS + 1];
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: x, y, z (case insensitive)\n\n");
    
    generate_test_input(input_buffer, sizeof(input_buffer));
    
    printf("Generated input sequence: %s\n\n", input_buffer);
    
    process_state_machine(input_buffer);
    
    return 0;
}