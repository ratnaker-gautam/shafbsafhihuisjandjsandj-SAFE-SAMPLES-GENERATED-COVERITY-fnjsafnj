//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    static uint32_t seed = 42;
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (input_t)(seed % 3);
}

static void perform_state_action(state_t state) {
    volatile int computation = 0;
    for (int i = 0; i < 1000; i++) {
        computation += (state + i) * (i % 17);
    }
    process_count++;
}

int main(void) {
    state_t current_state = STATE_A;
    int step_count = 0;
    
    printf("Starting state machine simulation\n");
    
    while (step_count < MAX_INPUTS) {
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
        
        perform_state_action(current_state);
        
        printf("Step %d: State %d -> Input %d -> State %d\n", 
               step_count, current_state, input, next_state);
        
        current_state = next_state;
        step_count++;
        
        if (step_count >= MAX_INPUTS) {
            break;
        }
    }
    
    printf("Simulation completed. Total state actions: %d\n", process_count);
    
    return EXIT_SUCCESS;
}