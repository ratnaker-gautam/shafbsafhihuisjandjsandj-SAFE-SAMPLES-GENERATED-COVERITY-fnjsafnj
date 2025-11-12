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

static void simulate_workload(StateMachine *machine) {
    for (int i = 0; i < 1000000; i++) {
        uint64_t temp = machine->counter;
        for (int j = 0; j < 100; j++) {
            temp = (temp * 1103515245 + 12345) % 2147483647;
        }
        machine->counter = temp;
    }
}

static void run_state_machine(StateMachine *machine, Input *inputs, size_t input_count) {
    for (size_t i = 0; i < input_count; i++) {
        process_input(machine, inputs[i]);
        simulate_workload(machine);
    }
}

static int parse_inputs(char *input_str, Input *inputs, size_t max_inputs) {
    if (input_str == NULL || inputs == NULL || max_inputs == 0) {
        return 0;
    }
    
    size_t len = strlen(input_str);
    if (len == 0 || len > max_inputs) {
        return 0;
    }
    
    size_t count = 0;
    for (size_t i = 0; i < len && count < max_inputs; i++) {
        switch (input_str[i]) {
            case 'x':
            case 'X':
                inputs[count++] = INPUT_X;
                break;
            case 'y':
            case 'Y':
                inputs[count++] = INPUT_Y;
                break;
            case 'z':
            case 'Z':
                inputs[count++] = INPUT_Z;
                break;
            default:
                return 0;
        }
    }
    
    return count;
}

int main(void) {
    char input_buffer[1024];
    Input inputs[MAX_INPUTS];
    StateMachine machine;
    
    printf("Enter sequence of inputs (x, y, z): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    int input_count = parse_inputs(input_buffer, inputs, MAX_INPUTS);
    if (input_count <= 0) {
        fprintf(stderr, "Invalid input sequence\n");
        return 1;
    }
    
    initialize_machine(&machine);
    run_state_machine(&machine, inputs, input_count);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total transitions: %u\n", machine.transitions);
    printf("Counter value: %lu\n", machine.counter);
    
    return 0;
}