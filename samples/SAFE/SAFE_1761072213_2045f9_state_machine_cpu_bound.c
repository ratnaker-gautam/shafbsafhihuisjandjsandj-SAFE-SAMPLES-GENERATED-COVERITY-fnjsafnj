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

static input_t generate_input(int sequence) {
    return (input_t)((sequence * 7 + 11) % 3);
}

static state_t process_state(state_t current, input_t input) {
    if (current >= STATE_COUNT || input > INPUT_Z) {
        return STATE_A;
    }
    return transition_table[current][input];
}

static void simulate_state_machine(int iterations) {
    if (iterations <= 0 || iterations > MAX_INPUTS) {
        printf("Invalid iteration count\n");
        return;
    }

    state_t current = STATE_A;
    int transitions = 0;

    for (int i = 0; i < iterations; i++) {
        input_t input = generate_input(i);
        state_t next = process_state(current, input);
        
        if (next >= STATE_COUNT) {
            next = STATE_A;
        }
        
        current = next;
        transitions++;
        
        for (int j = 0; j < 1000; j++) {
            process_count = (process_count * 3 + j) % 10007;
        }
    }

    printf("Final state: %d\n", current);
    printf("Total transitions: %d\n", transitions);
    printf("Process count: %d\n", process_count);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    char *endptr;
    long iterations = strtol(argv[1], &endptr, 10);
    
    if (*endptr != '\0' || iterations <= 0 || iterations > MAX_INPUTS) {
        printf("Invalid iterations: must be between 1 and %d\n", MAX_INPUTS);
        return 1;
    }

    simulate_state_machine((int)iterations);
    return 0;
}