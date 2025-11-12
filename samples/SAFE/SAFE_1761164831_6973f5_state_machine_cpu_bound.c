//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_COUNT
} Input;

static const State transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E"
};

static const char* input_names[INPUT_COUNT] = {
    "0", "1", "2"
};

State process_input(State current, Input input) {
    if (current >= STATE_COUNT || input >= INPUT_COUNT) {
        return STATE_A;
    }
    return transition_table[current][input];
}

int validate_input_sequence(const int* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < 0 || inputs[i] >= INPUT_COUNT) {
            return 0;
        }
    }
    return 1;
}

void simulate_state_machine(const int* inputs, size_t count) {
    if (!validate_input_sequence(inputs, count)) {
        printf("Invalid input sequence\n");
        return;
    }

    State current = STATE_A;
    printf("Initial state: %s\n", state_names[current]);

    for (size_t i = 0; i < count; i++) {
        Input input_val = (Input)inputs[i];
        State next = process_input(current, input_val);
        printf("Input: %s -> State: %s -> %s\n", 
               input_names[input_val], 
               state_names[current], 
               state_names[next]);
        current = next;
    }

    printf("Final state: %s\n", state_names[current]);
}

int main(void) {
    int inputs[MAX_INPUTS];
    size_t count = 0;
    char buffer[64];

    printf("Enter state machine inputs (0,1,2), one per line. Empty line to finish.\n");
    printf("Maximum %d inputs allowed.\n", MAX_INPUTS);

    while (count < MAX_INPUTS) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        if (buffer[0] == '\n') {
            break;
        }

        char* endptr;
        long value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter 0, 1, or 2.\n");
            continue;
        }

        if (value < 0 || value >= INPUT_COUNT) {
            printf("Input must be 0, 1, or 2.\n");
            continue;
        }

        inputs[count] = (int)value;
        count++;
    }

    if (count == 0) {
        printf("No valid inputs provided.\n");
        return 1;
    }

    simulate_state_machine(inputs, count);
    return 0;
}