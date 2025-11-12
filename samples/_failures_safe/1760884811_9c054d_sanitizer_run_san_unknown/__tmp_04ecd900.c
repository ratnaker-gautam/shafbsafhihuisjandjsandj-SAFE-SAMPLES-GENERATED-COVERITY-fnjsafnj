//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_INPUTS 1000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E
} State;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} Input;

typedef struct {
    State current_state;
    uint64_t counter;
    uint32_t transitions;
} StateMachine;

static State transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->counter = 0;
    machine->transitions = 0;
}

static Input get_next_input(uint32_t index) {
    const uint32_t pattern[] = {INPUT_X, INPUT_Y, INPUT_Z, INPUT_X, INPUT_Y, INPUT_Z, INPUT_X};
    return pattern[index % 7];
}

static void process_transition(StateMachine *machine, Input input) {
    if (machine->current_state >= STATE_COUNT || input > INPUT_Z) {
        return;
    }
    
    State next_state = transition_table[machine->current_state][input];
    if (next_state >= STATE_COUNT) {
        return;
    }
    
    machine->current_state = next_state;
    machine->transitions++;
    
    for (uint32_t i = 0; i < 1000; i++) {
        machine->counter += (machine->current_state + 1) * (input + 1);
        if (machine->counter > UINT64_MAX - 1000000) {
            machine->counter = 0;
        }
    }
}

static void print_state_info(const StateMachine *machine) {
    const char *state_names[] = {"A", "B", "C", "D", "E"};
    
    if (machine->current_state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[machine->current_state]);
    }
    printf("Transitions: %u\n", machine->transitions);
    printf("Counter value: %lu\n", machine->counter);
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    uint32_t num_inputs;
    printf("Enter number of state transitions (1-%u): ", MAX_INPUTS);
    
    if (scanf("%u", &num_inputs) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_inputs == 0 || num_inputs > MAX_INPUTS) {
        printf("Number out of range\n");
        return 1;
    }
    
    printf("Processing %u state transitions...\n", num_inputs);
    
    for (uint32_t i = 0; i < num_inputs; i++) {
        Input input = get_next_input(i);
        process_transition(&machine, input);
    }
    
    print_state_info(&machine);
    
    return 0;
}