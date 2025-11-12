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
    {STATE_B, STATE_A, STATE_C},
    {STATE_C, STATE_B, STATE_D},
    {STATE_D, STATE_C, STATE_E},
    {STATE_E, STATE_D, STATE_A},
    {STATE_A, STATE_E, STATE_B}
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
        result += i * i * i;
    }
    process_count[STATE_D]++;
}

static void process_state_e(void) {
    volatile int result = 0;
    for (int i = 0; i < 30000; i++) {
        result += i * i * (i + 1);
    }
    process_count[STATE_E]++;
}

static void execute_state_processing(state_t state) {
    switch (state) {
        case STATE_A:
            process_state_a();
            break;
        case STATE_B:
            process_state_b();
            break;
        case STATE_C:
            process_state_c();
            break;
        case STATE_D:
            process_state_d();
            break;
        case STATE_E:
            process_state_e();
            break;
        default:
            break;
    }
}

static input_t get_random_input(void) {
    static unsigned int seed = 0;
    if (seed == 0) {
        seed = 42;
    }
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (input_t)(seed % 3);
}

static void print_statistics(void) {
    printf("State processing statistics:\n");
    printf("State A: %d times\n", process_count[STATE_A]);
    printf("State B: %d times\n", process_count[STATE_B]);
    printf("State C: %d times\n", process_count[STATE_C]);
    printf("State D: %d times\n", process_count[STATE_D]);
    printf("State E: %d times\n", process_count[STATE_E]);
    
    int total = 0;
    for (int i = 0; i < STATE_COUNT; i++) {
        total += process_count[i];
    }
    printf("Total state executions: %d\n", total);
}

int main(void) {
    printf("Starting CPU-bound state machine simulation\n");
    
    state_t current_state = STATE_A;
    int step_count = 0;
    const int max_steps = 100;
    
    while (step_count < max_steps) {
        input_t input = get_random_input();
        
        if (input < 0 || input > 2) {
            fprintf(stderr, "Invalid input generated\n");
            return 1;
        }
        
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        execute_state_processing(current_state);
        
        printf("Step %d: State %d -> Input %d -> State %d\n", 
               step_count + 1, current_state, input, next_state);
        
        current_state = next_state;
        step_count++;
        
        if (step_count >= MAX_INPUTS) {
            fprintf(stderr, "Step count exceeded maximum\n");
            break;
        }
    }
    
    print_statistics();
    return 0;
}