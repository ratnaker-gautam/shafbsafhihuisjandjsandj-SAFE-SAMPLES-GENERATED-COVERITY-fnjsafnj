//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 64

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

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} input_t;

typedef struct {
    state_t current;
    input_t input;
    state_t next;
} transition_t;

static const transition_t transitions[] = {
    {STATE_A, INPUT_0, STATE_B}, {STATE_A, INPUT_1, STATE_C},
    {STATE_B, INPUT_0, STATE_D}, {STATE_B, INPUT_1, STATE_E},
    {STATE_C, INPUT_0, STATE_F}, {STATE_C, INPUT_1, STATE_G},
    {STATE_D, INPUT_0, STATE_H}, {STATE_D, INPUT_1, STATE_A},
    {STATE_E, INPUT_0, STATE_B}, {STATE_E, INPUT_1, STATE_C},
    {STATE_F, INPUT_0, STATE_D}, {STATE_F, INPUT_1, STATE_E},
    {STATE_G, INPUT_0, STATE_F}, {STATE_G, INPUT_1, STATE_G},
    {STATE_H, INPUT_0, STATE_H}, {STATE_H, INPUT_1, STATE_A}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D",
    "STATE_E", "STATE_F", "STATE_G", "STATE_H"
};

static const char* input_names[] = {
    "INPUT_0", "INPUT_1", "INPUT_2", "INPUT_3",
    "INPUT_4", "INPUT_5", "INPUT_6", "INPUT_7"
};

state_t process_transition(state_t current, input_t input) {
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void simulate_state_machine(uint32_t iterations) {
    if (iterations == 0 || iterations > 1000000) {
        printf("Invalid iteration count\n");
        return;
    }

    state_t current_state = STATE_A;
    uint32_t transition_count = 0;
    
    for (uint32_t i = 0; i < iterations; i++) {
        input_t input = (input_t)(i % 8);
        
        state_t next_state = process_transition(current_state, input);
        
        if (next_state != current_state) {
            transition_count++;
        }
        
        current_state = next_state;
        
        if (i % 10000 == 0) {
            printf("Iteration %u: State=%s, Input=%s, Transitions=%u\n",
                   i, state_names[current_state], input_names[input], transition_count);
        }
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Total transitions: %u\n", transition_count);
    printf("Total iterations: %u\n", iterations);
}

int main(void) {
    printf("State Machine Simulation\n");
    printf("========================\n");
    
    uint32_t iterations;
    printf("Enter number of iterations (1-1000000): ");
    
    if (scanf("%u", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    simulate_state_machine(iterations);
    
    return 0;
}