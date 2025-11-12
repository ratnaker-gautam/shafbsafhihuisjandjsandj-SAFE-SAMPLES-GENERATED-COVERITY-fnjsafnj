//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    int step_count = 0;
    int max_steps;
    
    printf("Enter number of state transitions (1-1000): ");
    if (scanf("%d", &max_steps) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (max_steps < 1 || max_steps > MAX_INPUTS) {
        fprintf(stderr, "Input out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state machine simulation...\n");
    
    while (step_count < max_steps) {
        input_t input = generate_input();
        
        if (input < 0 || input > 2) {
            fprintf(stderr, "Invalid input generated\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        process_state(current_state);
        
        printf("Step %d: State %d -> Input %d -> State %d\n", 
               step_count + 1, current_state, input, next_state);
        
        current_state = next_state;
        step_count++;
        
        if (step_count >= INT_MAX - 1) {
            fprintf(stderr, "Step count overflow\n");
            return EXIT_FAILURE;
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total steps processed: %d\n", step_count);
    
    return EXIT_SUCCESS;
}