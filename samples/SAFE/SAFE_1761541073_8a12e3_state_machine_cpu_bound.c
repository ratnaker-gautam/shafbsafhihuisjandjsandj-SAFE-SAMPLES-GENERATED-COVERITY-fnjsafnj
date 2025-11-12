//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    
    sm->transition_count = MAX_TRANSITIONS;
    sm->transitions[0] = (Transition){STATE_A, 0, STATE_B};
    sm->transitions[1] = (Transition){STATE_A, 1, STATE_C};
    sm->transitions[2] = (Transition){STATE_B, 0, STATE_D};
    sm->transitions[3] = (Transition){STATE_B, 1, STATE_E};
    sm->transitions[4] = (Transition){STATE_C, 0, STATE_F};
    sm->transitions[5] = (Transition){STATE_C, 1, STATE_G};
    sm->transitions[6] = (Transition){STATE_D, 0, STATE_H};
    sm->transitions[7] = (Transition){STATE_D, 1, STATE_A};
    sm->transitions[8] = (Transition){STATE_E, 0, STATE_B};
    sm->transitions[9] = (Transition){STATE_E, 1, STATE_C};
    sm->transitions[10] = (Transition){STATE_F, 0, STATE_D};
    sm->transitions[11] = (Transition){STATE_F, 1, STATE_E};
    sm->transitions[12] = (Transition){STATE_G, 0, STATE_F};
    sm->transitions[13] = (Transition){STATE_G, 1, STATE_G};
    sm->transitions[14] = (Transition){STATE_H, 0, STATE_H};
    sm->transitions[15] = (Transition){STATE_H, 1, STATE_A};
    
    sm->current_state = STATE_A;
}

State process_transition(StateMachine *sm, int input) {
    if (input < 0 || input >= MAX_INPUTS) {
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

int validate_iterations(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 0;
    }
    return 1;
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    int iterations;
    printf("Enter number of state transitions to simulate (1-%d): ", MAX_ITERATIONS);
    
    if (scanf("%d", &iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!validate_iterations(iterations)) {
        fprintf(stderr, "Invalid iteration count\n");
        return 1;
    }
    
    printf("Starting state machine simulation with %d transitions\n", iterations);
    printf("Initial state: %d\n", sm.current_state);
    
    int state_counts[MAX_STATES] = {0};
    state_counts[sm.current_state]++;
    
    for (int i = 0; i < iterations; i++) {
        int input = i % MAX_INPUTS;
        if (input < 0 || input >= MAX_INPUTS) {
            input = 0;
        }
        
        State new_state = process_transition(&sm, input);
        sm.current_state = new_state;
        state_counts[sm.current_state]++;
        
        if (i < 10 || i >= iterations - 10) {
            printf("Step %d: input=%d, state=%d\n", i, input, sm.current_state);
        }
    }
    
    printf("\nFinal state: %d\n", sm.current_state);
    printf("State distribution:\n");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("State %d: %d visits\n", i, state_counts[i]);
    }
    
    return 0;
}