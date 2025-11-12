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
    machine->counter++;
    machine->transitions++;
    
    if (machine->counter == UINT64_MAX) {
        machine->counter = 0;
    }
    if (machine->transitions == UINT32_MAX) {
        machine->transitions = 0;
    }
}

static void simulate_workload(uint32_t iterations) {
    volatile uint64_t result = 0;
    for (uint32_t i = 0; i < iterations; i++) {
        result += i * i;
        if (result > UINT64_MAX / 2) {
            result = 0;
        }
    }
}

static int parse_natural(const char *str, uint32_t *value) {
    if (str == NULL) {
        return 0;
    }
    
    char *endptr;
    unsigned long long temp = strtoull(str, &endptr, 10);
    
    if (*endptr != '\0') {
        return 0;
    }
    
    if (temp > UINT32_MAX) {
        return 0;
    }
    
    *value = (uint32_t)temp;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_iterations>\n", argv[0]);
        return 1;
    }
    
    uint32_t iterations;
    if (!parse_natural(argv[1], &iterations)) {
        fprintf(stderr, "Error: Invalid number of iterations\n");
        return 1;
    }
    
    if (iterations == 0 || iterations > MAX_INPUTS) {
        fprintf(stderr, "Error: Iterations must be between 1 and %d\n", MAX_INPUTS);
        return 1;
    }
    
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation with %u iterations\n", iterations);
    printf("Initial state: A\n");
    
    for (uint32_t i = 0; i < iterations; i++) {
        Input current_input = (Input)(i % 3);
        State previous_state = machine.current_state;
        
        process_input(&machine, current_input);
        
        printf("Step %u: Input %c, State %c -> %c\n", 
               i + 1, 
               'X' + current_input,
               'A' + previous_state,
               'A' + machine.current_state);
        
        if (i % 100 == 0) {
            simulate_workload(1000);
        }
    }
    
    printf("Final state: %c\n", 'A' + machine.current_state);
    printf("Total transitions: %u\n", machine.transitions);
    printf("Total operations: %lu\n", (unsigned long)machine.counter);
    
    return 0;
}