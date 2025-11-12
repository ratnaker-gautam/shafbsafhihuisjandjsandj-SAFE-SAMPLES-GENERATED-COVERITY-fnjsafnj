//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 128

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    int input;
    State next;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->state_count = MAX_STATES;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    sm->states[5] = STATE_F;
    sm->states[6] = STATE_G;
    sm->states[7] = STATE_H;
    
    sm->transition_count = 0;
    sm->current_state = STATE_A;
    
    int transitions[][3] = {
        {STATE_A, 1, STATE_B}, {STATE_A, 2, STATE_C},
        {STATE_B, 1, STATE_C}, {STATE_B, 2, STATE_D},
        {STATE_C, 1, STATE_D}, {STATE_C, 2, STATE_E},
        {STATE_D, 1, STATE_E}, {STATE_D, 2, STATE_F},
        {STATE_E, 1, STATE_F}, {STATE_E, 2, STATE_G},
        {STATE_F, 1, STATE_G}, {STATE_F, 2, STATE_H},
        {STATE_G, 1, STATE_H}, {STATE_G, 2, STATE_A},
        {STATE_H, 1, STATE_A}, {STATE_H, 2, STATE_B}
    };
    
    int num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    if (num_transitions > MAX_TRANSITIONS) {
        num_transitions = MAX_TRANSITIONS;
    }
    
    for (int i = 0; i < num_transitions; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count].current = transitions[i][0];
            sm->transitions[sm->transition_count].input = transitions[i][1];
            sm->transitions[sm->transition_count].next = transitions[i][2];
            sm->transition_count++;
        }
    }
}

State process_input(StateMachine *sm, int input) {
    if (input != 1 && input != 2) {
        return sm->current_state;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

void simulate_state_machine(StateMachine *sm, int iterations) {
    if (iterations <= 0 || iterations > 1000000) {
        iterations = 1000;
    }
    
    int input_sequence[] = {1, 2, 1, 2, 1, 1, 2, 2};
    int seq_length = sizeof(input_sequence) / sizeof(input_sequence[0]);
    
    for (int i = 0; i < iterations; i++) {
        int input = input_sequence[i % seq_length];
        State new_state = process_input(sm, input);
        
        volatile int computation = 0;
        for (int j = 0; j < 1000; j++) {
            computation += (input * j) % 7;
        }
    }
}

int main(int argc, char *argv[]) {
    int iterations = 10000;
    
    if (argc > 1) {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && *endptr == '\0' && val > 0 && val <= 1000000) {
            iterations = (int)val;
        }
    }
    
    StateMachine sm;
    initialize_machine(&sm);
    
    printf("Starting state machine simulation with %d iterations\n", iterations);
    printf("Initial state: %d\n", sm.current_state);
    
    simulate_state_machine(&sm, iterations);
    
    printf("Final state: %d\n", sm.current_state);
    printf("Simulation completed\n");
    
    return 0;
}