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

static int process_count = 0;

static input_t generate_input(void) {
    process_count++;
    uint32_t val = (process_count * 1103515245 + 12345) % 3;
    return (input_t)val;
}

static void process_state(state_t state) {
    volatile int computation = 0;
    for (int i = 0; i < 10000; i++) {
        computation += (state + i) * (i % 7);
    }
}

int main(void) {
    state_t current_state = STATE_A;
    int steps;
    
    printf("Enter number of state transitions (1-1000): ");
    if (scanf("%d", &steps) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (steps < 1 || steps > MAX_INPUTS) {
        fprintf(stderr, "Error: Steps must be between 1 and 1000\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state machine simulation...\n");
    printf("Initial state: A\n");
    
    for (int i = 0; i < steps; i++) {
        input_t input = generate_input();
        
        if (input < INPUT_X || input > INPUT_Z) {
            fprintf(stderr, "Error: Invalid input generated\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            fprintf(stderr, "Error: Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        process_state(current_state);
        
        printf("Step %d: State %c -> Input %c -> State %c\n", 
               i + 1, 
               'A' + current_state,
               'X' + input,
               'A' + next_state);
        
        current_state = next_state;
    }
    
    printf("Final state: %c\n", 'A' + current_state);
    printf("Total computations: %d\n", steps * 10000);
    
    return EXIT_SUCCESS;
}