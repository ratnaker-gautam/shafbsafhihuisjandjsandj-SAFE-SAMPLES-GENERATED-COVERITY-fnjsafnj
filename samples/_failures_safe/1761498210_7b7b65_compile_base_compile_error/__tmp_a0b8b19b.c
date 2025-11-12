//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4

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
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_A;
    sm->transition_count = 0;
}

int add_transition(StateMachine *sm, State from, int input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (input < 0 || input >= MAX_INPUTS) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, int input) {
    if (input < 0 || input >= MAX_INPUTS) {
        return sm->current_state;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return sm->current_state;
}

const char* state_to_string(State s) {
    switch(s) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        default: return "UNKNOWN";
    }
}

int validate_input_sequence(const int *sequence, int length) {
    if (sequence == NULL || length <= 0 || length > 1000) {
        return 0;
    }
    for (int i = 0; i < length; i++) {
        if (sequence[i] < 0 || sequence[i] >= MAX_INPUTS) {
            return 0;
        }
    }
    return 1;
}

void run_state_sequence(StateMachine *sm, const int *sequence, int length) {
    if (!validate_input_sequence(sequence, length)) {
        printf("Invalid input sequence\n");
        return;
    }
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    for (int i = 0; i < length; i++) {
        State previous = sm->current_state;
        State new_state = process_input(sm, sequence[i]);
        printf("Input: %d, Transition: %s -> %s\n", 
               sequence[i], state_to_string(previous), state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    add_transition(&sm, STATE_A, 0, STATE_B);
    add_transition(&sm, STATE_A, 1, STATE_C);
    add_transition(&sm, STATE_B, 0, STATE_D);
    add_transition(&sm, STATE_B, 1, STATE_E);
    add_transition(&sm, STATE_C, 0, STATE_F);
    add_transition(&sm, STATE_C, 1, STATE_G);
    add_transition(&sm, STATE_D, 0, STATE_H);
    add_transition(&sm, STATE_D, 1, STATE_A);
    add_transition(&sm, STATE_E, 0, STATE_B);
    add_transition(&sm, STATE_E, 1, STATE_C);
    add_transition(&sm, STATE_F, 0, STATE_D);
    add_transition(&sm, STATE_F, 1, STATE_E);
    add_transition(&sm, STATE_G, 0, STATE_F);
    add_transition(&sm, STATE_G, 1, STATE_G);
    add_transition(&sm, STATE_H, 0, STATE_H);
    add_transition(&sm, STATE_H, 1, STATE_A);
    
    int test_sequence[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    int seq_length = sizeof(test_sequence) / sizeof(test_sequence[0]);
    
    run_state_sequence(&sm, test_sequence, seq_length);
    
    int computation_cycles = 1000000;
    long state_count[MAX_STATES] = {0};
    
    for (int cycle = 0; cycle < computation_cycles; cycle++) {
        int random_input = cycle % MAX_INPUTS;
        State new_state = process_input(&sm, random_input);
        if (new_state >= 0 && new_state < MAX_STATES) {
            state_count[new_state]++;
        }
        
        if (cycle > 0