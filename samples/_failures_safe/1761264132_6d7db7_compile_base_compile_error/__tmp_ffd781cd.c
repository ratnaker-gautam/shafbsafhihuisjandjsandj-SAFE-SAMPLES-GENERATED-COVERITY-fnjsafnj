//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 100
#define MAX_INPUT_LENGTH 1000

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
    State from;
    State to;
    char input;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
    State accepting_states[MAX_STATES];
    int accepting_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->accepting_count = 0;
    
    for (int i = 0; i < MAX_STATES; i++) {
        machine->accepting_states[i] = STATE_A;
    }
}

int add_transition(StateMachine *machine, State from, State to, char input) {
    if (machine == NULL) return 0;
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from < 0 || from >= MAX_STATES || to < 0 || to >= MAX_STATES) {
        return 0;
    }
    
    machine->transitions[machine->transition_count].from = from;
    machine->transitions[machine->transition_count].to = to;
    machine->transitions[machine->transition_count].input = input;
    machine->transition_count++;
    return 1;
}

int add_accepting_state(StateMachine *machine, State state) {
    if (machine == NULL) return 0;
    if (machine->accepting_count >= MAX_STATES) {
        return 0;
    }
    if (state < 0 || state >= MAX_STATES) {
        return 0;
    }
    
    machine->accepting_states[machine->accepting_count] = state;
    machine->accepting_count++;
    return 1;
}

int is_accepting_state(StateMachine *machine, State state) {
    if (machine == NULL) return 0;
    for (int i = 0; i < machine->accepting_count; i++) {
        if (machine->accepting_states[i] == state) {
            return 1;
        }
    }
    return 0;
}

State process_input(StateMachine *machine, char input) {
    if (machine == NULL) return STATE_A;
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].from == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].to;
            return machine->current_state;
        }
    }
    return machine->current_state;
}

void setup_sample_machine(StateMachine *machine) {
    if (machine == NULL) return;
    add_transition(machine, STATE_A, STATE_B, '0');
    add_transition(machine, STATE_A, STATE_C, '1');
    add_transition(machine, STATE_B, STATE_D, '0');
    add_transition(machine, STATE_B, STATE_E, '1');
    add_transition(machine, STATE_C, STATE_F, '0');
    add_transition(machine, STATE_C, STATE_G, '1');
    add_transition(machine, STATE_D, STATE_H, '0');
    add_transition(machine, STATE_D, STATE_I, '1');
    add_transition(machine, STATE_E, STATE_J, '0');
    add_transition(machine, STATE_E, STATE_A, '1');
    add_transition(machine, STATE_F, STATE_B, '0');
    add_transition(machine, STATE_F, STATE_C, '1');
    add_transition(machine, STATE_G, STATE_D, '0');
    add_transition(machine, STATE_G, STATE_E, '1');
    add_transition(machine, STATE_H, STATE_F, '0');
    add_transition(machine, STATE_H, STATE_G, '1');
    add_transition(machine, STATE_I, STATE_H, '0');
    add_transition(machine, STATE_I, STATE_I, '1');
    add_transition(machine, STATE_J, STATE_J, '0');
    add_transition(machine, STATE_J, STATE_A, '1');
    
    add_accepting_state(machine, STATE_D);
    add_accepting_state(machine, STATE_G);
    add_accepting_state(machine, STATE_J);
}

int validate_input_string(const char *input) {
    if (input == NULL) {
        return 0;
    }
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LENGTH) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    return 1;
}

void process_string(StateMachine *machine, const char *input) {
    if (machine == NULL) {
        printf("Invalid state machine\n");
        return;
    }
    if (!validate_input_string(input)) {
        printf("Invalid