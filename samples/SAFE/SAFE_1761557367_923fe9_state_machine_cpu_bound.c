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
    size_t transition_count;
} StateMachine;

void initialize_machine(StateMachine *machine, State initial) {
    if (machine == NULL) return;
    machine->current_state = initial;
    machine->transition_count = 0;
}

int add_transition(StateMachine *machine, State from, Input input, State to) {
    if (machine == NULL) return 0;
    if (machine->transition_count >= MAX_TRANSITIONS) return 0;
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) return 0;
    
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, Input input) {
    if (machine == NULL) return machine->current_state;
    if (input >= MAX_INPUTS) return machine->current_state;
    
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return machine->current_state;
        }
    }
    return machine->current_state;
}

void generate_test_inputs(Input *inputs, size_t count) {
    if (inputs == NULL || count == 0) return;
    
    uint32_t seed = 42;
    for (size_t i = 0; i < count; i++) {
        inputs[i] = (Input)(seed % MAX_INPUTS);
        seed = seed * 1103515245 + 12345;
    }
}

int main(void) {
    StateMachine fsm;
    initialize_machine(&fsm, STATE_A);
    
    add_transition(&fsm, STATE_A, INPUT_0, STATE_B);
    add_transition(&fsm, STATE_A, INPUT_1, STATE_C);
    add_transition(&fsm, STATE_B, INPUT_2, STATE_D);
    add_transition(&fsm, STATE_B, INPUT_3, STATE_E);
    add_transition(&fsm, STATE_C, INPUT_4, STATE_F);
    add_transition(&fsm, STATE_C, INPUT_5, STATE_G);
    add_transition(&fsm, STATE_D, INPUT_6, STATE_H);
    add_transition(&fsm, STATE_E, INPUT_7, STATE_A);
    add_transition(&fsm, STATE_F, INPUT_0, STATE_B);
    add_transition(&fsm, STATE_G, INPUT_1, STATE_C);
    add_transition(&fsm, STATE_H, INPUT_2, STATE_D);
    
    const size_t num_inputs = 1000000;
    Input *test_inputs = malloc(num_inputs * sizeof(Input));
    if (test_inputs == NULL) {
        return 1;
    }
    
    generate_test_inputs(test_inputs, num_inputs);
    
    State final_state = fsm.current_state;
    for (size_t i = 0; i < num_inputs; i++) {
        final_state = process_input(&fsm, test_inputs[i]);
    }
    
    printf("Final state: %d\n", (int)final_state);
    printf("Processed %zu inputs\n", num_inputs);
    
    free(test_inputs);
    return 0;
}