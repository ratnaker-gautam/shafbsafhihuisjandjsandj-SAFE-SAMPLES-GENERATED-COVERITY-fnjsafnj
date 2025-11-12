//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 32

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
            break;
        }
    }
    return sm->current_state;
}

void generate_input_sequence(int *sequence, int length, unsigned int seed) {
    if (length <= 0 || sequence == NULL) {
        return;
    }
    for (int i = 0; i < length; i++) {
        sequence[i] = (seed * (i + 1)) % MAX_INPUTS;
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
    add_transition(&sm, STATE_E, INPUT_7, STATE_A);
    add_transition(&sm, STATE_F, INPUT_0, STATE_B);
    add_transition(&sm, STATE_G, INPUT_1, STATE_C);
    add_transition(&sm, STATE_H, INPUT_2, STATE_D);
    
    const int sequence_length = 1000000;
    int *input_sequence = malloc(sequence_length * sizeof(int));
    if (input_sequence == NULL) {
        return 1;
    }
    
    generate_input_sequence(input_sequence, sequence_length, 42);
    
    for (int i = 0; i < sequence_length; i++) {
        if (input_sequence[i] < 0 || input_sequence[i] >= MAX_INPUTS) {
            continue;
        }
        process_input(&sm, (Input)input_sequence[i]);
    }
    
    printf("Final state: %d\n", (int)sm.current_state);
    
    free(input_sequence);
    return 0;
}