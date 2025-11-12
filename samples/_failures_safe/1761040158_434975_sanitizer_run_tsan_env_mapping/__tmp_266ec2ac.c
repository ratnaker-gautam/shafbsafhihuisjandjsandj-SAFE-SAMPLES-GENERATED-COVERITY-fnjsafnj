//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

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

void simulate_machine(StateMachine *sm, const char *input) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        State previous = sm->current_state;
        State new_state = process_input(sm, input[i]);
        printf("Input '%c': State %d -> %d\n", input[i], previous, new_state);
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
    add_transition(&sm, STATE_D, '1', STATE_I);
    add_transition(&sm, STATE_E, '0', STATE_J);
    add_transition(&sm, STATE_E, '1', STATE_A);
    add_transition(&sm, STATE_F, '0', STATE_B);
    add_transition(&sm, STATE_F, '1', STATE_C);
    add_transition(&sm, STATE_G, '0', STATE_D);
    add_transition(&sm, STATE_G, '1', STATE_E);
    add_transition(&sm, STATE_H, '0', STATE_F);
    add_transition(&sm, STATE_H, '1', STATE_G);
    add_transition(&sm, STATE_I, '0', STATE_H);
    add_transition(&sm, STATE_I, '1', STATE_I);
    add_transition(&sm, STATE_J, '0', STATE_J);
    add_transition(&sm, STATE_J, '1', STATE_A);
    
    char input[MAX_INPUT_LEN + 1];
    printf("Enter binary sequence (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            printf("Invalid input: only '0' and '1' allowed\n");
            return 1;
        }
    }
    
    printf("Initial state: %d\n", sm.current_state);
    simulate_machine(&sm, input);
    printf("Final state: %d\n", sm.current_state);
    
    return 0;
}