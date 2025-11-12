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

static int process_count[STATE_COUNT] = {0};

static void process_state_a(void) {
    volatile int result = 0;
    for (int i = 0; i < 10000; i++) {
        result += i * i;
    }
    process_count[STATE_A]++;
}

static void process_state_b(void) {
    volatile int result = 0;
    for (int i = 0; i < 15000; i++) {
        result += i * (i + 1);
    }
    process_count[STATE_B]++;
}

static void process_state_c(void) {
    volatile int result = 0;
    for (int i = 0; i < 20000; i++) {
        result += i * (i - 1);
    }
    process_count[STATE_C]++;
}

static void process_state_d(void) {
    volatile int result = 0;
    for (int i = 0; i < 25000; i++) {
        result += i * (i + 2);
    }
    process_count[STATE_D]++;
}

static void process_state_e(void) {
    volatile int result = 0;
    for (int i = 0; i < 30000; i++) {
        result += i * (i - 2);
    }
    process_count[STATE_E]++;
}

static void execute_state(state_t state) {
    switch (state) {
        case STATE_A: process_state_a(); break;
        case STATE_B: process_state_b(); break;
        case STATE_C: process_state_c(); break;
        case STATE_D: process_state_d(); break;
        case STATE_E: process_state_e(); break;
    }
}

static input_t get_next_input(void) {
    static int counter = 0;
    input_t inputs[] = {INPUT_X, INPUT_Y, INPUT_Z};
    return inputs[counter++ % 3];
}

static void print_statistics(void) {
    printf("State execution counts:\n");
    printf("State A: %d\n", process_count[STATE_A]);
    printf("State B: %d\n", process_count[STATE_B]);
    printf("State C: %d\n", process_count[STATE_C]);
    printf("State D: %d\n", process_count[STATE_D]);
    printf("State E: %d\n", process_count[STATE_E]);
}

int main(void) {
    state_t current_state = STATE_A;
    int iterations;
    
    printf("Enter number of state machine iterations (1-1000): ");
    if (scanf("%d", &iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (iterations < 1 || iterations > MAX_INPUTS) {
        fprintf(stderr, "Iterations must be between 1 and 1000\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state machine with %d iterations\n", iterations);
    
    for (int i = 0; i < iterations; i++) {
        input_t input = get_next_input();
        state_t next_state = transition_table[current_state][input];
        
        printf("Step %d: State %d -> Input %d -> State %d\n", 
               i + 1, current_state, input, next_state);
        
        execute_state(current_state);
        current_state = next_state;
    }
    
    print_statistics();
    return EXIT_SUCCESS;
}