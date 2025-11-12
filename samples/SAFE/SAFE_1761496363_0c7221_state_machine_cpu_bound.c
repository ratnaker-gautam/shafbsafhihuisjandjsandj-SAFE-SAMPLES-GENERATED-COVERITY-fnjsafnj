//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUTS 1000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H,
    STATE_I,
    STATE_J
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
    sm->state_count = 5;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    
    sm->transition_count = 8;
    sm->transitions[0] = (Transition){STATE_A, 0, STATE_B};
    sm->transitions[1] = (Transition){STATE_A, 1, STATE_C};
    sm->transitions[2] = (Transition){STATE_B, 0, STATE_D};
    sm->transitions[3] = (Transition){STATE_B, 1, STATE_E};
    sm->transitions[4] = (Transition){STATE_C, 0, STATE_A};
    sm->transitions[5] = (Transition){STATE_C, 1, STATE_B};
    sm->transitions[6] = (Transition){STATE_D, 0, STATE_E};
    sm->transitions[7] = (Transition){STATE_E, 1, STATE_A};
    
    sm->current_state = STATE_A;
}

State process_transition(StateMachine *sm, int input) {
    if (input != 0 && input != 1) {
        return sm->current_state;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    
    return sm->current_state;
}

void generate_inputs(int *inputs, int count) {
    for (int i = 0; i < count; i++) {
        inputs[i] = i % 2;
    }
}

int validate_state(State state) {
    return state >= STATE_A && state <= STATE_J;
}

int main(void) {
    StateMachine sm;
    int inputs[MAX_INPUTS];
    int iterations = 1000000;
    
    if (iterations <= 0 || iterations > MAX_INPUTS) {
        fprintf(stderr, "Invalid iteration count\n");
        return EXIT_FAILURE;
    }
    
    initialize_machine(&sm);
    generate_inputs(inputs, iterations);
    
    if (!validate_state(sm.current_state)) {
        fprintf(stderr, "Invalid initial state\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < iterations; i++) {
        State new_state = process_transition(&sm, inputs[i]);
        if (!validate_state(new_state)) {
            fprintf(stderr, "Invalid state transition\n");
            return EXIT_FAILURE;
        }
        sm.current_state = new_state;
        
        for (int j = 0; j < 100; j++) {
            int temp = inputs[i] * j;
            temp = temp % 100;
        }
    }
    
    printf("Final state: %d\n", sm.current_state);
    printf("Processed %d state transitions\n", iterations);
    
    return EXIT_SUCCESS;
}