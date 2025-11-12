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

uint32_t simple_hash(uint32_t x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int main(void) {
    printf("State Machine Simulation (CPU Bound)\n");
    printf("Running %d iterations...\n", MAX_ITERATIONS);
    
    state_t current_state = STATE_A;
    uint32_t state_counts[MAX_STATES] = {0};
    uint32_t seed = 12345;
    uint32_t iterations = 0;
    
    for (uint32_t i = 0; i < MAX_ITERATIONS; i++) {
        if (i >= MAX_ITERATIONS) {
            break;
        }
        
        seed = simple_hash(seed + i);
        uint8_t input = (seed >> 8) % MAX_INPUTS;
        
        if (input >= MAX_INPUTS) {
            input = MAX_INPUTS - 1;
        }
        
        state_t next_state = get_next_state(current_state, input);
        
        if (next_state >= MAX_STATES) {
            next_state = STATE_A;
        }
        
        if (state_counts[next_state] < UINT32_MAX) {
            state_counts[next_state]++;
        }
        
        current_state = next_state;
        iterations++;
    }
    
    printf("Completed %u iterations\n", iterations);
    printf("Final state distribution:\n");
    
    for (state_t s = STATE_A; s < MAX_STATES; s++) {
        if (s >= MAX_STATES) {
            break;
        }
        printf("State %s: %u visits\n", state_names[s], state_counts[s]);
    }
    
    uint32_t total = 0;
    for (state_t s = STATE_A; s < MAX_STATES; s++) {
        if (s >= MAX_STATES) {
            break;
        }
        if (total <= UINT32_MAX - state_counts[s]) {
            total += state_counts[s];
        } else {
            total = UINT32_MAX;
            break;
        }
    }
    
    printf("Total state visits: %u\n", total);
    
    return 0;
}