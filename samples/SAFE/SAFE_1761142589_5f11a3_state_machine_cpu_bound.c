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
    INPUT_2
} Input;

State transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

int validate_input(const char* input_str, Input* inputs, size_t max_inputs) {
    if (input_str == NULL) return 0;
    
    size_t len = strlen(input_str);
    if (len == 0 || len > max_inputs) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input_str[i];
        if (c < '0' || c > '2') return 0;
        inputs[i] = (Input)(c - '0');
    }
    
    return 1;
}

void process_state_machine(State initial_state, Input* inputs, size_t input_count) {
    State current_state = initial_state;
    unsigned long long computation = 1;
    
    for (size_t i = 0; i < input_count; i++) {
        if (current_state >= STATE_COUNT || inputs[i] > INPUT_2) {
            fprintf(stderr, "Invalid state or input\n");
            return;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        for (int j = 0; j < 1000; j++) {
            computation = (computation * 3) % 1000000007ULL;
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Computation result: %llu\n", computation);
}

int main(void) {
    char input_buffer[1024];
    Input inputs[MAX_INPUTS];
    
    printf("Enter input sequence (0-2 only, max %d characters): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer, inputs, MAX_INPUTS)) {
        fprintf(stderr, "Invalid input sequence\n");
        return 1;
    }
    
    size_t input_count = strlen(input_buffer);
    
    printf("Processing state machine with %zu inputs...\n", input_count);
    process_state_machine(STATE_A, inputs, input_count);
    
    return 0;
}