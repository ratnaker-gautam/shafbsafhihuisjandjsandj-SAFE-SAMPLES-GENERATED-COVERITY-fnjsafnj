//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_STATES 16
#define MAX_TRANSITIONS 64
#define MAX_INPUTS 8

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
    char input;
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
    if (sm == NULL) {
        return;
    }
    
    sm->state_count = 8;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    sm->states[5] = STATE_F;
    sm->states[6] = STATE_G;
    sm->states[7] = STATE_H;
    
    sm->transition_count = 16;
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_E};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_F};
    sm->transitions[5] = (Transition){STATE_C, '1', STATE_G};
    sm->transitions[6] = (Transition){STATE_D, '0', STATE_H};
    sm->transitions[7] = (Transition){STATE_D, '1', STATE_A};
    sm->transitions[8] = (Transition){STATE_E, '0', STATE_B};
    sm->transitions[9] = (Transition){STATE_E, '1', STATE_C};
    sm->transitions[10] = (Transition){STATE_F, '0', STATE_D};
    sm->transitions[11] = (Transition){STATE_F, '1', STATE_E};
    sm->transitions[12] = (Transition){STATE_G, '0', STATE_F};
    sm->transitions[13] = (Transition){STATE_G, '1', STATE_G};
    sm->transitions[14] = (Transition){STATE_H, '0', STATE_H};
    sm->transitions[15] = (Transition){STATE_H, '1', STATE_A};
    
    sm->current_state = STATE_A;
}

int process_transition(StateMachine *sm, char input) {
    if (sm == NULL) {
        return 0;
    }
    
    if (input != '0' && input != '1') {
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

void run_state_machine(StateMachine *sm, const char *input_sequence) {
    if (sm == NULL || input_sequence == NULL) {
        return;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 1000) {
        return;
    }
    
    int valid_transitions = 0;
    for (size_t i = 0; i < len; i++) {
        if (process_transition(sm, input_sequence[i])) {
            valid_transitions++;
        }
    }
    
    printf("Processed %d valid transitions\n", valid_transitions);
    printf("Final state: %d\n", (int)sm->current_state);
}

void generate_test_sequence(char *buffer, size_t length) {
    if (buffer == NULL || length == 0) {
        return;
    }
    
    if (length > 1000) {
        length = 1000;
    }
    
    for (size_t i = 0; i < length - 1; i++) {
        buffer[i] = (rand() % 2) ? '1' : '0';
    }
    buffer[length - 1] = '\0';
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    StateMachine sm;
    initialize_machine(&sm);
    
    char test_sequence[1001];
    generate_test_sequence(test_sequence, sizeof(test_sequence));
    
    printf("Input sequence: %s\n", test_sequence);
    printf("Initial state: %d\n", (int)sm.current_state);
    
    run_state_machine(&sm, test_sequence);
    
    int iterations = 10000;
    int total_transitions = 0;
    
    for (int i = 0; i < iterations; i++) {
        StateMachine temp_sm;
        initialize_machine(&temp_sm);
        
        char seq[101];
        generate_test_sequence(seq, sizeof(seq));
        
        size_t len = strlen(seq);
        int transitions = 0;
        for