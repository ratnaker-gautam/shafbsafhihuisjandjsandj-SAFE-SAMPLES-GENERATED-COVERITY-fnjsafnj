//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 50
#define MAX_INPUTS 1000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    NUM_STATES
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

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(StateMachine *machine, State from, char input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= NUM_STATES || to >= NUM_STATES) {
        return 0;
    }
    
    Transition *t = &machine->transitions[machine->transition_count];
    t->current = from;
    t->input = input;
    t->next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, char input) {
    for (int i = 0; i < machine->transition_count; i++) {
        Transition *t = &machine->transitions[i];
        if (t->current == machine->current_state && t->input == input) {
            machine->current_state = t->next;
            return t->next;
        }
    }
    return machine->current_state;
}

void generate_test_input(char *inputs, int count) {
    const char valid_chars[] = "01";
    int num_chars = strlen(valid_chars);
    
    for (int i = 0; i < count; i++) {
        inputs[i] = valid_chars[rand() % num_chars];
    }
}

int main(void) {
    StateMachine machine;
    char inputs[MAX_INPUTS];
    int num_inputs = 500;
    
    if (num_inputs > MAX_INPUTS) {
        num_inputs = MAX_INPUTS;
    }
    
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, '0', STATE_B);
    add_transition(&machine, STATE_A, '1', STATE_C);
    add_transition(&machine, STATE_B, '0', STATE_D);
    add_transition(&machine, STATE_B, '1', STATE_A);
    add_transition(&machine, STATE_C, '0', STATE_A);
    add_transition(&machine, STATE_C, '1', STATE_E);
    add_transition(&machine, STATE_D, '0', STATE_E);
    add_transition(&machine, STATE_D, '1', STATE_B);
    add_transition(&machine, STATE_E, '0', STATE_C);
    add_transition(&machine, STATE_E, '1', STATE_D);
    
    generate_test_input(inputs, num_inputs);
    
    int state_counts[NUM_STATES] = {0};
    
    for (int i = 0; i < num_inputs; i++) {
        State new_state = process_input(&machine, inputs[i]);
        if (new_state < NUM_STATES) {
            state_counts[new_state]++;
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("State distribution:\n");
    for (int i = 0; i < NUM_STATES; i++) {
        printf("State %d: %d visits\n", i, state_counts[i]);
    }
    
    int total_transitions = 0;
    for (int i = 0; i < num_inputs; i++) {
        for (int j = 0; j < machine.transition_count; j++) {
            if (machine.transitions[j].input == inputs[i]) {
                total_transitions++;
            }
        }
    }
    
    printf("Total transition evaluations: %d\n", total_transitions);
    
    return 0;
}