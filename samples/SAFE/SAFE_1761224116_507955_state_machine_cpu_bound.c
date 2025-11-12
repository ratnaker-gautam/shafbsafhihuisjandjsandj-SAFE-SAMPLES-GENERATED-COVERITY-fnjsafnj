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
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[INPUT_COUNT] = {
    "INPUT_0", "INPUT_1", "INPUT_2"
};

int validate_input_range(int value) {
    return value >= 0 && value < INPUT_COUNT;
}

int process_state_machine(int* inputs, int input_count) {
    if (inputs == NULL || input_count <= 0 || input_count > MAX_INPUTS) {
        return EXIT_FAILURE;
    }

    State current_state = STATE_A;
    int valid_transitions = 0;

    for (int i = 0; i < input_count; i++) {
        if (!validate_input_range(inputs[i])) {
            return EXIT_FAILURE;
        }

        Input current_input = (Input)inputs[i];
        State next_state = transition_table[current_state][current_input];
        
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            return EXIT_FAILURE;
        }

        printf("Step %d: %s + %s -> %s\n", 
               i + 1, 
               state_names[current_state],
               input_names[current_input],
               state_names[next_state]);

        current_state = next_state;
        valid_transitions++;
    }

    printf("Final state: %s\n", state_names[current_state]);
    printf("Total valid transitions: %d\n", valid_transitions);
    
    return EXIT_SUCCESS;
}

int main(void) {
    int input_count;
    
    printf("Enter number of inputs (1-%d): ", MAX_INPUTS);
    if (scanf("%d", &input_count) != 1) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }

    if (input_count <= 0 || input_count > MAX_INPUTS) {
        printf("Error: Input count out of range\n");
        return EXIT_FAILURE;
    }

    int* inputs = malloc((size_t)input_count * sizeof(int));
    if (inputs == NULL) {
        printf("Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Enter %d inputs (0-2):\n", input_count);
    for (int i = 0; i < input_count; i++) {
        printf("Input %d: ", i + 1);
        if (scanf("%d", &inputs[i]) != 1) {
            printf("Error: Invalid input format\n");
            free(inputs);
            return EXIT_FAILURE;
        }
    }

    int result = process_state_machine(inputs, input_count);
    free(inputs);
    
    return result;
}