//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} state_t;

typedef struct {
    state_t current;
    uint8_t input;
    state_t next;
} transition_t;

static transition_t transitions[MAX_TRANSITIONS] = {
    {STATE_A, 0, STATE_B}, {STATE_A, 1, STATE_C},
    {STATE_B, 0, STATE_D}, {STATE_B, 1, STATE_E},
    {STATE_C, 0, STATE_F}, {STATE_C, 1, STATE_G},
    {STATE_D, 0, STATE_H}, {STATE_D, 1, STATE_A},
    {STATE_E, 0, STATE_B}, {STATE_E, 1, STATE_C},
    {STATE_F, 0, STATE_D}, {STATE_F, 1, STATE_E},
    {STATE_G, 0, STATE_F}, {STATE_G, 1, STATE_G},
    {STATE_H, 0, STATE_H}, {STATE_H, 1, STATE_A}
};

static const char* state_names[MAX_STATES] = {
    "A", "B", "C", "D", "E", "F", "G", "H"
};

state_t get_next_state(state_t current, uint8_t input) {
    for (size_t i = 0; i < MAX_TRANSITIONS; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

uint64_t process_state_machine(uint32_t iterations) {
    if (iterations == 0 || iterations > MAX_ITERATIONS) {
        return 0;
    }
    
    state_t current_state = STATE_A;
    uint64_t state_counts[MAX_STATES] = {0};
    uint32_t input_sequence = 0x5A5A5A5A;
    
    for (uint32_t i = 0; i < iterations; i++) {
        uint8_t input = (input_sequence >> (i % 32)) & 1;
        state_counts[current_state]++;
        current_state = get_next_state(current_state, input);
    }
    
    uint64_t result = 0;
    for (size_t i = 0; i < MAX_STATES; i++) {
        result ^= state_counts[i];
        result = (result << 7) | (result >> 57);
    }
    
    return result;
}

int main(void) {
    printf("State Machine CPU Benchmark\n");
    printf("Processing state transitions...\n");
    
    clock_t start_time = clock();
    if (start_time == (clock_t)-1) {
        fprintf(stderr, "Error: Could not get start time\n");
        return 1;
    }
    
    uint64_t result = process_state_machine(1000000);
    
    clock_t end_time = clock();
    if (end_time == (clock_t)-1) {
        fprintf(stderr, "Error: Could not get end time\n");
        return 1;
    }
    
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Final state machine result: %lu\n", result);
    printf("CPU time used: %.6f seconds\n", cpu_time);
    printf("State transitions per second: %.0f\n", 1000000.0 / cpu_time);
    
    return 0;
}