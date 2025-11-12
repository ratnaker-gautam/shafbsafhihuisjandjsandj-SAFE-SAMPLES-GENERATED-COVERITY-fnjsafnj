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
    {STATE_B, STATE_A, STATE_C},
    {STATE_C, STATE_B, STATE_D},
    {STATE_D, STATE_C, STATE_E},
    {STATE_E, STATE_D, STATE_A},
    {STATE_A, STATE_E, STATE_B}
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
    
    if (machine->counter < UINT64_MAX - 1) {
        machine->counter += (uint64_t)(machine->current_state + 1) * (input + 1);
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

static void run_state_machine(StateMachine *machine, int *inputs, int count) {
    if (count <= 0 || count > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (validate_input(inputs[i])) {
            process_input(machine, inputs[i]);
        }
    }
}

static void print_results(const StateMachine *machine) {
    const char *state_names[] = {"A", "B", "C", "D", "E"};
    
    printf("Final state: %s\n", state_names[machine->current_state]);
    printf("Total transitions: %u\n", machine->transitions);
    printf("Counter value: %lu\n", machine->counter);
}

int main(void) {
    StateMachine machine;
    int inputs[MAX_INPUTS];
    int input_count;
    
    srand(42);
    
    printf("Enter number of inputs to generate (1-%d): ", MAX_INPUTS);
    if (scanf("%d", &input_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input_count <= 0 || input_count > MAX_INPUTS) {
        printf("Input count out of range\n");
        return 1;
    }
    
    initialize_machine(&machine);
    generate_inputs(inputs, input_count);
    run_state_machine(&machine, inputs, input_count);
    print_results(&machine);
    
    return 0;
}