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

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} Input;

typedef struct {
    State current;
    Input input;
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

int add_transition(StateMachine *sm, State from, Input input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, Input input) {
    if (input >= MAX_INPUTS) {
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

void simulate_workload(StateMachine *sm, int iterations) {
    if (iterations <= 0 || iterations > 1000000) {
        iterations = 1000;
    }
    
    Input inputs[] = {INPUT_0, INPUT_1, INPUT_2, INPUT_3, INPUT_4, INPUT_5, INPUT_6, INPUT_7};
    int input_count = sizeof(inputs) / sizeof(inputs[0]);
    
    for (int i = 0; i < iterations; i++) {
        Input current_input = inputs[i % input_count];
        process_input(sm, current_input);
        
        for (int j = 0; j < 100; j++) {
            int temp = i * j;
            temp = temp % 1000;
        }
    }
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    add_transition(&sm, STATE_A, INPUT_0, STATE_B);
    add_transition(&sm, STATE_A, INPUT_1, STATE_C);
    add_transition(&sm, STATE_B, INPUT_0, STATE_C);
    add_transition(&sm, STATE_B, INPUT_1, STATE_D);
    add_transition(&sm, STATE_C, INPUT_0, STATE_D);
    add_transition(&sm, STATE_C, INPUT_1, STATE_E);
    add_transition(&sm, STATE_D, INPUT_0, STATE_E);
    add_transition(&sm, STATE_D, INPUT_1, STATE_F);
    add_transition(&sm, STATE_E, INPUT_0, STATE_F);
    add_transition(&sm, STATE_E, INPUT_1, STATE_G);
    add_transition(&sm, STATE_F, INPUT_0, STATE_G);
    add_transition(&sm, STATE_F, INPUT_1, STATE_H);
    add_transition(&sm, STATE_G, INPUT_0, STATE_H);
    add_transition(&sm, STATE_G, INPUT_1, STATE_A);
    add_transition(&sm, STATE_H, INPUT_0, STATE_A);
    add_transition(&sm, STATE_H, INPUT_1, STATE_B);
    
    printf("Starting state machine simulation...\n");
    simulate_workload(&sm, 50000);
    printf("Final state: %d\n", sm.current_state);
    
    return 0;
}