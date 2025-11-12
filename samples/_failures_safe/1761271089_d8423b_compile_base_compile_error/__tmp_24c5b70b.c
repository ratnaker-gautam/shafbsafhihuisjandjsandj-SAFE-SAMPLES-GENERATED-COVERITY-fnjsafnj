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
    char input;
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

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES) {
        return 0;
    }
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
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
    switch (s) {
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

void perform_cpu_work(int iterations) {
    volatile double result = 1.0;
    for (int i = 0; i < iterations; i++) {
        result = result * 1.000001 + 0.000001;
        if (result > 1000000.0) {
            result = 1.0;
        }
    }
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    add_transition(&sm, STATE_A, '0', STATE_B);
    add_transition(&sm, STATE_A, '1', STATE_C);
    add_transition(&sm, STATE_B, '0', STATE_D);
    add_transition(&sm, STATE_B, '1', STATE_E);
    add_transition(&sm, STATE_C, '0', STATE_F);
    add_transition(&sm, STATE_C, '1', STATE_G);
    add_transition(&sm, STATE_D, '0', STATE_H);
    add_transition(&sm, STATE_D, '1', STATE_A);
    add_transition(&sm, STATE_E, '0', STATE_B);
    add_transition(&sm, STATE_E, '1', STATE_C);
    add_transition(&sm, STATE_F, '0', STATE_D);
    add_transition(&sm, STATE_F, '1', STATE_E);
    add_transition(&sm, STATE_G, '0', STATE_F);
    add_transition(&sm, STATE_G, '1', STATE_G);
    add_transition(&sm, STATE_H, '0', STATE_H);
    add_transition(&sm, STATE_H, '1', STATE_A);
    
    char input_sequence[] = "0101101001011010";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len == 0 || seq_len > 1000) {
        fprintf(stderr, "Invalid input sequence length\n");
        return 1;
    }
    
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for (size_t i = 0; i < seq_len; i++) {
        if (input_sequence[i] != '0' && input_sequence[i] != '1') {
            fprintf(stderr, "Invalid input character: %c\n", input_sequence[i]);
            return 1;
        }
        
        State new_state = process_input(&sm, input_sequence[i]);
        perform_cpu_work(10000);
        
        printf("Input: %c -> New state: %s\n", 
               input_sequence[i], state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm.current_state));
    
    int state_count[MAX_STATES] = {0};
    for (int i = 0; i < 1000000; i++) {
        char random_input = (rand() % 2) ? '1' : '0';
        State new_state = process_input(&sm, random_input);
        if (new_state < MAX_STATES) {
            state_count[new_state]++;
        }
        perform_cpu_work(10);
    }
    
    printf("\nState distribution after random inputs:\n");
    for (int i = 0; i < MAX_STATES; i++) {
        printf("State %s: %d\n", state_to_string(i), state_count