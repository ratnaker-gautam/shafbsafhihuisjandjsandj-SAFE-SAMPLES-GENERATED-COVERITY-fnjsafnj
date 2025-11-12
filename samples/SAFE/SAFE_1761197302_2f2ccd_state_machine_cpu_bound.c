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

int process_input(StateMachine *sm, Input input) {
    if (input >= MAX_INPUTS) {
        return 0;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void generate_input_sequence(uint32_t seed, Input *sequence, int length) {
    if (length <= 0 || sequence == NULL) {
        return;
    }
    for (int i = 0; i < length; i++) {
        sequence[i] = (Input)(seed % MAX_INPUTS);
        seed = seed * 1103515245 + 12345;
    }
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    add_transition(&sm, STATE_A, INPUT_0, STATE_B);
    add_transition(&sm, STATE_A, INPUT_1, STATE_C);
    add_transition(&sm, STATE_B, INPUT_2, STATE_D);
    add_transition(&sm, STATE_B, INPUT_3, STATE_E);
    add_transition(&sm, STATE_C, INPUT_4, STATE_F);
    add_transition(&sm, STATE_C, INPUT_5, STATE_G);
    add_transition(&sm, STATE_D, INPUT_6, STATE_H);
    add_transition(&sm, STATE_D, INPUT_7, STATE_A);
    add_transition(&sm, STATE_E, INPUT_0, STATE_C);
    add_transition(&sm, STATE_E, INPUT_1, STATE_D);
    add_transition(&sm, STATE_F, INPUT_2, STATE_G);
    add_transition(&sm, STATE_F, INPUT_3, STATE_H);
    add_transition(&sm, STATE_G, INPUT_4, STATE_A);
    add_transition(&sm, STATE_G, INPUT_5, STATE_B);
    add_transition(&sm, STATE_H, INPUT_6, STATE_C);
    add_transition(&sm, STATE_H, INPUT_7, STATE_D);
    
    const int sequence_length = 1000000;
    Input *input_sequence = malloc(sequence_length * sizeof(Input));
    if (input_sequence == NULL) {
        return 1;
    }
    
    generate_input_sequence(42, input_sequence, sequence_length);
    
    int valid_transitions = 0;
    for (int i = 0; i < sequence_length; i++) {
        if (process_input(&sm, input_sequence[i])) {
            valid_transitions++;
        }
    }
    
    printf("Final state: %d\n", sm.current_state);
    printf("Valid transitions: %d\n", valid_transitions);
    
    free(input_sequence);
    return 0;
}