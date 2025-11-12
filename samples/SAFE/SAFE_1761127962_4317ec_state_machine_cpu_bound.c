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

static input_t get_next_input(void) {
    static int counter = 0;
    if (counter >= MAX_INPUTS) {
        return INPUT_X;
    }
    int val = (counter * 17 + 23) % 3;
    counter++;
    return (input_t)val;
}

static void process_state(state_t state) {
    volatile int computation = 0;
    for (int i = 0; i < 10000; i++) {
        computation += (state * i) % 100;
        computation = computation % 1000;
    }
    process_count++;
}

int main(void) {
    state_t current_state = STATE_A;
    int step_count = 0;
    const int max_steps = 100;
    
    printf("Starting state machine simulation\n");
    
    while (step_count < max_steps) {
        input_t input = get_next_input();
        
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
               step_count, current_state, input, next_state);
        
        current_state = next_state;
        step_count++;
        
        if (step_count > MAX_INPUTS) {
            fprintf(stderr, "Step count exceeded maximum\n");
            return EXIT_FAILURE;
        }
    }
    
    printf("Simulation completed. Total state processes: %d\n", process_count);
    
    return EXIT_SUCCESS;
}