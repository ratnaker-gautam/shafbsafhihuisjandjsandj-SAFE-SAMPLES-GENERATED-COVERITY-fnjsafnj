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

static input_t generate_input(int iteration) {
    return (input_t)(iteration % 3);
}

static state_t process_state(state_t current, input_t input) {
    if (current >= STATE_COUNT || input > INPUT_Z) {
        return STATE_A;
    }
    return transition_table[current][input];
}

static int validate_transitions(void) {
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < 3; j++) {
            state_t next = transition_table[i][j];
            if (next >= STATE_COUNT) {
                return 0;
            }
        }
    }
    return 1;
}

static void run_state_machine(int iterations) {
    if (iterations <= 0 || iterations > MAX_INPUTS) {
        return;
    }
    
    state_t current = STATE_A;
    process_count = 0;
    
    for (int i = 0; i < iterations; i++) {
        input_t input = generate_input(i);
        state_t next = process_state(current, input);
        
        if (next >= STATE_COUNT) {
            next = STATE_A;
        }
        
        current = next;
        process_count++;
        
        for (int j = 0; j < 1000; j++) {
            process_count = (process_count * 13 + 7) % 1000000;
        }
    }
}

int main(void) {
    if (!validate_transitions()) {
        return 1;
    }
    
    int iterations;
    if (scanf("%d", &iterations) != 1) {
        return 1;
    }
    
    if (iterations <= 0 || iterations > MAX_INPUTS) {
        return 1;
    }
    
    run_state_machine(iterations);
    
    printf("%d\n", process_count);
    return 0;
}