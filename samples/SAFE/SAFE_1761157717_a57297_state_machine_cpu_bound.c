//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_SYMBOLS 10

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H,
    STATE_I,
    STATE_J
} State;

typedef struct {
    State current_state;
    char input_symbol;
    State next_state;
    int output_value;
} Transition;

Transition state_table[] = {
    {STATE_A, '0', STATE_B, 1},
    {STATE_A, '1', STATE_C, 2},
    {STATE_B, '0', STATE_D, 3},
    {STATE_B, '1', STATE_E, 4},
    {STATE_C, '0', STATE_F, 5},
    {STATE_C, '1', STATE_G, 6},
    {STATE_D, '0', STATE_H, 7},
    {STATE_D, '1', STATE_I, 8},
    {STATE_E, '0', STATE_J, 9},
    {STATE_E, '1', STATE_A, 10},
    {STATE_F, '0', STATE_B, 11},
    {STATE_F, '1', STATE_C, 12},
    {STATE_G, '0', STATE_D, 13},
    {STATE_G, '1', STATE_E, 14},
    {STATE_H, '0', STATE_F, 15},
    {STATE_H, '1', STATE_G, 16},
    {STATE_I, '0', STATE_H, 17},
    {STATE_I, '1', STATE_I, 18},
    {STATE_J, '0', STATE_J, 19},
    {STATE_J, '1', STATE_A, 20}
};

#define NUM_TRANSITIONS (sizeof(state_table) / sizeof(state_table[0]))

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    return 1;
}

State process_transition(State current, char symbol, int* output) {
    for (size_t i = 0; i < NUM_TRANSITIONS; i++) {
        if (state_table[i].current_state == current && 
            state_table[i].input_symbol == symbol) {
            if (output != NULL) {
                *output = state_table[i].output_value;
            }
            return state_table[i].next_state;
        }
    }
    return current;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter binary sequence (max %d characters, only 0s and 1s): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input: must be non-empty binary sequence (max %d chars)\n", MAX_INPUT_LEN);
        return 1;
    }
    
    State current_state = STATE_A;
    int total_output = 0;
    int cycle_count = 0;
    const int max_cycles = 1000000;
    
    printf("Processing state machine...\n");
    
    for (int cycle = 0; cycle < max_cycles; cycle++) {
        for (size_t i = 0; i < len; i++) {
            int output_val = 0;
            State next_state = process_transition(current_state, input_buffer[i], &output_val);
            
            if (next_state == current_state) {
                fprintf(stderr, "No transition found for state %d, symbol '%c'\n", current_state, input_buffer[i]);
                return 1;
            }
            
            total_output += output_val;
            current_state = next_state;
            
            if (total_output < 0) {
                total_output = 0;
            }
        }
        cycle_count++;
        
        if (current_state == STATE_A && cycle_count >= 100) {
            break;
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total cycles processed: %d\n", cycle_count);
    printf("Accumulated output: %d\n", total_output);
    
    return 0;
}