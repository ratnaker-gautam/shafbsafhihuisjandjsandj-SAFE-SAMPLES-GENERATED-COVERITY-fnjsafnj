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
} state_t;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} input_t;

typedef struct {
    state_t current;
    input_t input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    size_t transition_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

static int add_transition(state_machine_t *machine, state_t from, input_t input, state_t to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

static state_t process_input(state_machine_t *machine, input_t input) {
    if (input >= MAX_INPUTS) {
        return machine->current_state;
    }
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            break;
        }
    }
    return machine->current_state;
}

static void run_state_machine(state_machine_t *machine, input_t *inputs, size_t input_count) {
    if (inputs == NULL || input_count == 0) {
        return;
    }
    for (size_t i = 0; i < input_count; i++) {
        process_input(machine, inputs[i]);
    }
}

static void populate_transitions(state_machine_t *machine) {
    add_transition(machine, STATE_A, INPUT_0, STATE_B);
    add_transition(machine, STATE_A, INPUT_1, STATE_C);
    add_transition(machine, STATE_B, INPUT_0, STATE_D);
    add_transition(machine, STATE_B, INPUT_1, STATE_E);
    add_transition(machine, STATE_C, INPUT_0, STATE_F);
    add_transition(machine, STATE_C, INPUT_1, STATE_G);
    add_transition(machine, STATE_D, INPUT_0, STATE_H);
    add_transition(machine, STATE_D, INPUT_1, STATE_A);
    add_transition(machine, STATE_E, INPUT_0, STATE_B);
    add_transition(machine, STATE_E, INPUT_1, STATE_C);
    add_transition(machine, STATE_F, INPUT_0, STATE_D);
    add_transition(machine, STATE_F, INPUT_1, STATE_E);
    add_transition(machine, STATE_G, INPUT_0, STATE_F);
    add_transition(machine, STATE_G, INPUT_1, STATE_G);
    add_transition(machine, STATE_H, INPUT_0, STATE_H);
    add_transition(machine, STATE_H, INPUT_1, STATE_A);
}

static const char *state_to_string(state_t state) {
    switch (state) {
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

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    populate_transitions(&machine);
    
    input_t test_inputs[] = {
        INPUT_0, INPUT_1, INPUT_0, INPUT_1, INPUT_0,
        INPUT_1, INPUT_0, INPUT_0, INPUT_1, INPUT_1,
        INPUT_0, INPUT_1, INPUT_0, INPUT_0, INPUT_1
    };
    size_t input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    printf("Initial state: %s\n", state_to_string(machine.current_state));
    
    for (size_t i = 0; i < input_count; i++) {
        state_t new_state = process_input(&machine, test_inputs[i]);
        printf("Input %zu -> State: %s\n", i, state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(machine.current_state));
    
    state_machine_t machine2;
    initialize_machine(&machine2);
    populate_transitions(&machine2);
    
    input_t bulk_inputs[1000];
    for (size_t i = 0; i < 1000; i++) {
        bulk_inputs[i] = (