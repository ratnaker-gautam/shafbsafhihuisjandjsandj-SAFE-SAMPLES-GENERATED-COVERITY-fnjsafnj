//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
    {STATE_B, STATE_A, STATE_C},
    {STATE_C, STATE_B, STATE_D},
    {STATE_D, STATE_C, STATE_E},
    {STATE_E, STATE_D, STATE_A},
    {STATE_A, STATE_E, STATE_B}
};

static int process_count = 0;

static input_t generate_input(int iteration) {
    return (input_t)((iteration * 7 + 11) % 3);
}

static void process_state(state_t state, int iteration) {
    volatile int computation = iteration;
    for (int i = 0; i < 1000; i++) {
        computation = (computation * 13 + 7) % 1000000;
    }
    process_count += computation % 100;
}

int main(void) {
    state_t current_state = STATE_A;
    int iterations;
    
    printf("Enter number of iterations (1-1000): ");
    if (scanf("%d", &iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (iterations < 1 || iterations > MAX_INPUTS) {
        fprintf(stderr, "Iterations out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state machine with %d iterations\n", iterations);
    
    for (int i = 0; i < iterations; i++) {
        input_t input = generate_input(i);
        state_t next_state = transition_table[current_state][input];
        
        process_state(current_state, i);
        
        printf("Step %d: State %d -> Input %d -> State %d\n", 
               i, current_state, input, next_state);
        
        current_state = next_state;
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total processing count: %d\n", process_count);
    
    return EXIT_SUCCESS;
}