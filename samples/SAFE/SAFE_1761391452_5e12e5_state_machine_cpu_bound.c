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
} state_t;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

static const state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[3] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

int validate_input_sequence(const input_t* inputs, size_t count) {
    if (inputs == NULL) return 0;
    if (count > MAX_INPUTS) return 0;
    return 1;
}

input_t generate_input(size_t index) {
    return (input_t)(index % 3);
}

void process_state_machine(size_t step_count) {
    if (step_count == 0 || step_count > MAX_INPUTS) {
        fprintf(stderr, "Invalid step count\n");
        return;
    }

    input_t inputs[MAX_INPUTS];
    for (size_t i = 0; i < step_count; i++) {
        inputs[i] = generate_input(i);
    }

    if (!validate_input_sequence(inputs, step_count)) {
        fprintf(stderr, "Input validation failed\n");
        return;
    }

    state_t current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);

    for (size_t i = 0; i < step_count; i++) {
        input_t current_input = inputs[i];
        if (current_input < 0 || current_input > 2) {
            fprintf(stderr, "Invalid input encountered\n");
            return;
        }

        state_t next_state = transition_table[current_state][current_input];
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return;
        }

        printf("Step %zu: Input %s -> State %s\n", 
               i + 1, input_names[current_input], state_names[next_state]);
        current_state = next_state;
    }

    printf("Final state: %s\n", state_names[current_state]);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <step_count>\n", argv[0]);
        return 1;
    }

    char* endptr;
    long steps = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || steps <= 0 || steps > MAX_INPUTS) {
        fprintf(stderr, "Invalid step count. Must be between 1 and %d\n", MAX_INPUTS);
        return 1;
    }

    process_state_machine((size_t)steps);
    return 0;
}