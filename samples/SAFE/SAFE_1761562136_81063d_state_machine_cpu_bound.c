//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_INPUTS 4
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} state_t;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3
} input_t;

typedef struct {
    state_t current_state;
    state_t next_state;
    int output;
} transition_t;

static transition_t state_table[MAX_STATES][MAX_INPUTS] = {
    {{STATE_A, STATE_B, 1}, {STATE_A, STATE_C, 2}, {STATE_A, STATE_D, 3}, {STATE_A, STATE_E, 4}},
    {{STATE_B, STATE_C, 5}, {STATE_B, STATE_D, 6}, {STATE_B, STATE_E, 7}, {STATE_B, STATE_F, 8}},
    {{STATE_C, STATE_D, 9}, {STATE_C, STATE_E, 10}, {STATE_C, STATE_F, 11}, {STATE_C, STATE_G, 12}},
    {{STATE_D, STATE_E, 13}, {STATE_D, STATE_F, 14}, {STATE_D, STATE_G, 15}, {STATE_D, STATE_H, 16}},
    {{STATE_E, STATE_F, 17}, {STATE_E, STATE_G, 18}, {STATE_E, STATE_H, 19}, {STATE_E, STATE_A, 20}},
    {{STATE_F, STATE_G, 21}, {STATE_F, STATE_H, 22}, {STATE_F, STATE_A, 23}, {STATE_F, STATE_B, 24}},
    {{STATE_G, STATE_H, 25}, {STATE_G, STATE_A, 26}, {STATE_G, STATE_B, 27}, {STATE_G, STATE_C, 28}},
    {{STATE_H, STATE_A, 29}, {STATE_H, STATE_B, 30}, {STATE_H, STATE_C, 31}, {STATE_H, STATE_D, 32}}
};

static int validate_state(state_t state) {
    return state >= STATE_A && state <= STATE_H;
}

static int validate_input(input_t input) {
    return input >= INPUT_0 && input <= INPUT_3;
}

static input_t generate_input(state_t state, uint64_t counter) {
    return (input_t)((state + counter) % MAX_INPUTS);
}

int main(void) {
    state_t current_state = STATE_A;
    uint64_t total_output = 0;
    uint64_t iterations = 0;
    
    printf("Starting state machine simulation...\n");
    
    while (iterations < MAX_ITERATIONS) {
        if (!validate_state(current_state)) {
            fprintf(stderr, "Invalid state detected\n");
            return 1;
        }
        
        input_t input = generate_input(current_state, iterations);
        
        if (!validate_input(input)) {
            fprintf(stderr, "Invalid input generated\n");
            return 1;
        }
        
        transition_t transition = state_table[current_state][input];
        
        if (!validate_state(transition.next_state)) {
            fprintf(stderr, "Invalid transition state\n");
            return 1;
        }
        
        if (transition.current_state != current_state) {
            fprintf(stderr, "State table inconsistency\n");
            return 1;
        }
        
        total_output += transition.output;
        current_state = transition.next_state;
        iterations++;
        
        if (total_output > UINT64_MAX - 1000) {
            total_output = 0;
        }
    }
    
    printf("State machine completed %lu iterations\n", (unsigned long)iterations);
    printf("Total output sum: %lu\n", (unsigned long)total_output);
    
    return 0;
}