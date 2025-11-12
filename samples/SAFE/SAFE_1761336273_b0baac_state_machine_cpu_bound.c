//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
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
    if (from >= MAX_STATES || to >= MAX_STATES) {
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
            break;
        }
    }
    
    return sm->current_state;
}

void run_state_machine(StateMachine *sm, int *inputs, int input_count) {
    if (inputs == NULL || input_count <= 0) {
        return;
    }
    
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] >= MAX_INPUTS) {
            continue;
        }
        process_input(sm, inputs[i]);
    }
}

int validate_input_sequence(int *inputs, int count) {
    if (inputs == NULL || count <= 0) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= MAX_INPUTS) {
            return 0;
        }
    }
    return 1;
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    add_transition(&sm, STATE_A, 0, STATE_B);
    add_transition(&sm, STATE_A, 1, STATE_C);
    add_transition(&sm, STATE_B, 0, STATE_D);
    add_transition(&sm, STATE_B, 1, STATE_A);
    add_transition(&sm, STATE_C, 0, STATE_A);
    add_transition(&sm, STATE_C, 1, STATE_D);
    add_transition(&sm, STATE_D, 0, STATE_E);
    add_transition(&sm, STATE_D, 1, STATE_F);
    add_transition(&sm, STATE_E, 0, STATE_G);
    add_transition(&sm, STATE_E, 1, STATE_H);
    add_transition(&sm, STATE_F, 0, STATE_H);
    add_transition(&sm, STATE_F, 1, STATE_G);
    add_transition(&sm, STATE_G, 0, STATE_A);
    add_transition(&sm, STATE_G, 1, STATE_B);
    add_transition(&sm, STATE_H, 0, STATE_C);
    add_transition(&sm, STATE_H, 1, STATE_D);
    
    int test_inputs[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    if (!validate_input_sequence(test_inputs, input_count)) {
        return 1;
    }
    
    printf("Initial state: %d\n", sm.current_state);
    
    for (int i = 0; i < input_count; i++) {
        State previous = sm.current_state;
        State new_state = process_input(&sm, test_inputs[i]);
        printf("Input: %d, Transition: %d -> %d\n", test_inputs[i], previous, new_state);
    }
    
    printf("Final state: %d\n", sm.current_state);
    
    int iterations = 100000;
    for (int iter = 0; iter < iterations; iter++) {
        StateMachine temp_sm;
        initialize_machine(&temp_sm);
        temp_sm.transition_count = sm.transition_count;
        for (int i = 0; i < sm.transition_count; i++) {
            temp_sm.transitions[i] = sm.transitions[i];
        }
        
        for (int i = 0; i < input_count; i++) {
            process_input(&temp_sm, test_inputs[i % input_count]);
        }
    }
    
    return 0;
}