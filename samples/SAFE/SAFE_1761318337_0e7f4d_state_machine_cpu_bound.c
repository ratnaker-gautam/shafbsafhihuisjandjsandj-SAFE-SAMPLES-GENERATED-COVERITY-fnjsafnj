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
    int value = (counter * 17 + 13) % 3;
    counter++;
    return (input_t)value;
}

static void process_state(state_t state) {
    volatile int computation = 0;
    for (int i = 0; i < 10000; i++) {
        computation += (state * i) % 100;
        computation = computation % 1000;
    }
    process_count += computation % 10;
}

int main(void) {
    state_t current_state = STATE_A;
    int step_count = 0;
    
    printf("Starting state machine simulation\n");
    
    while (step_count < MAX_INPUTS) {
        input_t input = get_next_input();
        
        if (input < 0 || input > 2) {
            fprintf(stderr, "Invalid input generated\n");
            return 1;
        }
        
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        process_state(current_state);
        
        current_state = next_state;
        step_count++;
        
        if (step_count % 100 == 0) {
            printf("Step %d: State %d, Process count: %d\n", 
                   step_count, current_state, process_count);
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total process operations: %d\n", process_count);
    printf("Simulation completed after %d steps\n", step_count);
    
    return 0;
}