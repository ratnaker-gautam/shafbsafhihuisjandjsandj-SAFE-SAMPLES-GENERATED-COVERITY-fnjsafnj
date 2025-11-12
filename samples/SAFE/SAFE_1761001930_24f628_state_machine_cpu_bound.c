//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

Transition transitions[] = {
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

#define NUM_TRANSITIONS (sizeof(transitions)/sizeof(transitions[0]))

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

int find_transition(State current_state, char input_symbol) {
    for (size_t i = 0; i < NUM_TRANSITIONS; i++) {
        if (transitions[i].current_state == current_state && 
            transitions[i].input_symbol == input_symbol) {
            return (int)i;
        }
    }
    return -1;
}

void process_state_machine(const char* input) {
    State current_state = STATE_A;
    int total_output = 0;
    size_t input_len = strlen(input);
    
    printf("Starting state: A\n");
    
    for (size_t i = 0; i < input_len; i++) {
        int transition_idx = find_transition(current_state, input[i]);
        if (transition_idx == -1) {
            printf("Invalid transition from state %d with symbol '%c'\n", 
                   current_state, input[i]);
            return;
        }
        
        Transition* trans = &transitions[transition_idx];
        current_state = trans->next_state;
        total_output += trans->output_value;
        
        printf("Input '%c': State %d -> State %d, Output: %d, Total: %d\n",
               input[i], trans->current_state, trans->next_state, 
               trans->output_value, total_output);
    }
    
    printf("Final state: %d, Total output: %d\n", current_state, total_output);
}

int main() {
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter binary sequence (max %d characters, only 0s and 1s): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input. Please enter only 0s and 1s (max %d characters)\n", MAX_INPUT_LEN);
        return 1;
    }
    
    process_state_machine(input);
    
    return 0;
}