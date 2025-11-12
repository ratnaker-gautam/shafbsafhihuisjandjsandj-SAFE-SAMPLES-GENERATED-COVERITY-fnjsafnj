//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    state_t current_state;
    uint8_t input;
    state_t next_state;
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

static state_t process_transition(state_t current, uint8_t input) {
    for (size_t i = 0; i < MAX_TRANSITIONS; i++) {
        if (transitions[i].current_state == current && transitions[i].input == input) {
            return transitions[i].next_state;
        }
    }
    return current;
}

static uint8_t generate_input(uint64_t counter) {
    return (counter % 3) == 0 ? 1 : 0;
}

static void print_state_name(state_t state) {
    const char* names[] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    if (state < MAX_STATES) {
        printf("%s", names[state]);
    }
}

int main(void) {
    state_t current_state = STATE_A;
    uint64_t iterations = 0;
    uint64_t max_iterations = MAX_ITERATIONS;
    
    printf("Starting state machine simulation\n");
    printf("Initial state: ");
    print_state_name(current_state);
    printf("\n");
    
    while (iterations < max_iterations) {
        uint8_t input = generate_input(iterations);
        state_t next_state = process_transition(current_state, input);
        
        if (iterations % 100000 == 0) {
            printf("Iteration %lu: State ", iterations);
            print_state_name(current_state);
            printf(" -> Input %u -> State ", input);
            print_state_name(next_state);
            printf("\n");
        }
        
        current_state = next_state;
        iterations++;
        
        if (current_state == STATE_H && iterations > max_iterations / 2) {
            break;
        }
    }
    
    printf("Final state: ");
    print_state_name(current_state);
    printf(" after %lu iterations\n", iterations);
    
    uint64_t state_counts[MAX_STATES] = {0};
    current_state = STATE_A;
    iterations = 0;
    
    while (iterations < 10000) {
        state_counts[current_state]++;
        uint8_t input = generate_input(iterations);
        current_state = process_transition(current_state, input);
        iterations++;
    }
    
    printf("State distribution after 10000 steps:\n");
    for (state_t s = STATE_A; s < MAX_STATES; s++) {
        printf("State ");
        print_state_name(s);
        printf(": %lu visits\n", state_counts[s]);
    }
    
    return 0;
}