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

static Transition transitions[] = {
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

static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static int find_transition(State current, char symbol) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].input_symbol == symbol) {
            return i;
        }
    }
    return -1;
}

static int validate_input(const char* input) {
    if (input == NULL) {
        return 0;
    }
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    return 1;
}

static void process_state_machine(const char* input) {
    State current_state = STATE_A;
    int total_output = 0;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        int transition_idx = find_transition(current_state, input[i]);
        if (transition_idx == -1) {
            printf("Invalid transition at position %zu\n", i);
            return;
        }
        
        total_output += transitions[transition_idx].output_value;
        current_state = transitions[transition_idx].next_state;
        
        if (total_output < 0) {
            printf("Output overflow detected\n");
            return;
        }
    }
    
    printf("Final state: %d, Total output: %d\n", (int)current_state, total_output);
}

static void run_cpu_bound_operations(int iterations) {
    if (iterations <= 0 || iterations > 1000000) {
        printf("Invalid iteration count\n");
        return;
    }
    
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 100; j++) {
            result += (i * j) % 100;
            if (result < 0) {
                result = 0;
            }
        }
    }
    
    printf("CPU bound result: %d\n", result);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter binary input string (max %d characters, only 0s and 1s): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input)) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Processing state machine...\n");
    process_state_machine(input);
    
    printf("Running CPU bound operations...\n");
    run_cpu_bound_operations(10000);
    
    return 0;
}