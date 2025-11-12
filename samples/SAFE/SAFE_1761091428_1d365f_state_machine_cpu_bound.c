//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

void simulate_computation(int iterations) {
    volatile uint64_t result = 0;
    for (int i = 0; i < iterations; i++) {
        result = (result * 1103515245 + 12345) % 2147483647;
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
    
    char input_sequence[64];
    printf("Enter binary sequence (max 63 chars, only 0/1): ");
    
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input sequence\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input_sequence[i] != '0' && input_sequence[i] != '1') {
            fprintf(stderr, "Invalid character in sequence: %c\n", input_sequence[i]);
            return 1;
        }
    }
    
    printf("Initial state: A\n");
    printf("Processing sequence: %s\n", input_sequence);
    
    for (size_t i = 0; i < len; i++) {
        State previous = sm.current_state;
        State new_state = process_input(&sm, input_sequence[i]);
        simulate_computation(100000);
        
        const char *state_names[] = {"A", "B", "C", "D", "E", "F", "G", "H"};
        printf("Input: %c | State: %s -> %s\n", 
               input_sequence[i], state_names[previous], state_names[new_state]);
    }
    
    const char *state_names[] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    printf("Final state: %s\n", state_names[sm.current_state]);
    
    return 0;
}