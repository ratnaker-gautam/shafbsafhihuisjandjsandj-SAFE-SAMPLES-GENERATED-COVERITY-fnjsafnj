//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static int validate_input(int input) {
    return (input >= 0 && input <= 2);
}

static void process_input(StateMachine *machine, Input input) {
    if (machine->transitions == UINT32_MAX) {
        return;
    }
    
    State next_state = transition_table[machine->current_state][input];
    machine->current_state = next_state;
    machine->counter++;
    machine->transitions++;
}

static void generate_inputs(int *inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        inputs[i] = rand() % 3;
    }
}

static void run_simulation(StateMachine *machine, const int *inputs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (!validate_input(inputs[i])) {
            continue;
        }
        process_input(machine, (Input)inputs[i]);
        
        if (machine->counter == UINT64_MAX) {
            break;
        }
    }
}

int main(void) {
    StateMachine machine;
    int inputs[MAX_INPUTS];
    
    if (MAX_INPUTS == 0) {
        return EXIT_FAILURE;
    }
    
    initialize_machine(&machine);
    
    srand(42);
    generate_inputs(inputs, MAX_INPUTS);
    
    run_simulation(&machine, inputs, MAX_INPUTS);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total transitions: %u\n", machine.transitions);
    printf("Counter value: %lu\n", machine.counter);
    
    return EXIT_SUCCESS;
}