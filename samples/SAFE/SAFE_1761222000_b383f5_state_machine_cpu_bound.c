//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 128

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

typedef struct {
    state_t current;
    int input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
    int input_history[MAX_INPUTS];
    int input_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    machine->input_count = 0;
    
    transition_t trans[] = {
        {STATE_A, 1, STATE_B}, {STATE_A, 2, STATE_C},
        {STATE_B, 1, STATE_C}, {STATE_B, 3, STATE_D},
        {STATE_C, 2, STATE_E}, {STATE_C, 4, STATE_F},
        {STATE_D, 3, STATE_G}, {STATE_D, 5, STATE_H},
        {STATE_E, 4, STATE_A}, {STATE_E, 6, STATE_B},
        {STATE_F, 5, STATE_C}, {STATE_F, 7, STATE_D},
        {STATE_G, 6, STATE_E}, {STATE_G, 8, STATE_F},
        {STATE_H, 7, STATE_G}, {STATE_H, 9, STATE_A}
    };
    
    int trans_count = sizeof(trans) / sizeof(trans[0]);
    if (trans_count > MAX_TRANSITIONS) {
        trans_count = MAX_TRANSITIONS;
    }
    
    for (int i = 0; i < trans_count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count++] = trans[i];
        }
    }
}

static state_t process_input(state_machine_t *machine, int input) {
    if (input < 0 || input > 9) {
        return machine->current_state;
    }
    
    if (machine->input_count < MAX_INPUTS) {
        machine->input_history[machine->input_count++] = input;
    }
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            break;
        }
    }
    
    return machine->current_state;
}

static void simulate_state_transitions(state_machine_t *machine) {
    int test_inputs[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 1, 2, 3, 4, 5, 6, 7};
    int input_count = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    if (input_count > MAX_INPUTS) {
        input_count = MAX_INPUTS;
    }
    
    for (int i = 0; i < input_count; i++) {
        process_input(machine, test_inputs[i]);
    }
}

static void perform_computation(state_machine_t *machine) {
    unsigned long computation = 0;
    
    for (int i = 0; i < machine->input_count; i++) {
        int val = machine->input_history[i];
        for (int j = 0; j < 1000; j++) {
            computation = (computation * 31 + val) % 1000000007;
        }
    }
    
    printf("Computation result: %lu\n", computation);
}

static void print_state_info(state_machine_t *machine) {
    const char *state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D",
        "STATE_E", "STATE_F", "STATE_G", "STATE_H"
    };
    
    printf("Final state: %s\n", state_names[machine->current_state]);
    printf("Processed inputs: %d\n", machine->input_count);
    
    printf("Input sequence: ");
    for (int i = 0; i < machine->input_count; i++) {
        printf("%d ", machine->input_history[i]);
    }
    printf("\n");
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine);
    simulate_state_transitions(&machine);
    perform_computation(&machine);
    print_state_info(&machine);
    
    return 0;
}