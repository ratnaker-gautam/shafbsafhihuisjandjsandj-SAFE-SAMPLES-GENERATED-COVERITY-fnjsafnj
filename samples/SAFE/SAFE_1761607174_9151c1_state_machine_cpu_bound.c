//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static const State transition_table[STATE_COUNT][3] = {
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
    return input >= INPUT_X && input <= INPUT_Z;
}

static void process_input(StateMachine *machine, Input input) {
    if (!validate_input(input)) {
        return;
    }
    
    State next_state = transition_table[machine->current_state][input];
    machine->current_state = next_state;
    machine->transitions++;
    
    for (int i = 0; i < 1000; i++) {
        machine->counter += (machine->current_state + 1) * (input + 1);
        if (machine->counter > UINT64_MAX - 1000000) {
            machine->counter = 0;
        }
    }
}

static void generate_inputs(int *inputs, int count) {
    if (count <= 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        inputs[i] = rand() % 3;
    }
}

static void run_simulation(StateMachine *machine, int iterations) {
    if (iterations <= 0 || iterations > MAX_INPUTS) {
        return;
    }
    
    int inputs[MAX_INPUTS];
    generate_inputs(inputs, iterations);
    
    for (int i = 0; i < iterations; i++) {
        process_input(machine, inputs[i]);
    }
}

static const char* state_to_string(State state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    int iterations;
    printf("Enter number of iterations (1-1000): ");
    
    if (scanf("%d", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (iterations < 1 || iterations > MAX_INPUTS) {
        printf("Iterations must be between 1 and 1000\n");
        return 1;
    }
    
    run_simulation(&machine, iterations);
    
    printf("Final state: %s\n", state_to_string(machine.current_state));
    printf("Total transitions: %u\n", machine.transitions);
    printf("Counter value: %lu\n", machine.counter);
    
    return 0;
}