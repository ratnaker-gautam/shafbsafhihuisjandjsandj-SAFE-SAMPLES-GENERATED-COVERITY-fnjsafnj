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

void process_state_machine(const int* inputs, size_t count) {
    if (!validate_input_sequence(inputs, count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return;
    }

    State current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);

    for (size_t i = 0; i < count; i++) {
        Input input = (Input)inputs[i];
        State next_state = transition_table[current_state][input];
        
        printf("Input: %s -> Transition: %s -> %s\n",
               input_names[input],
               state_names[current_state],
               state_names[next_state]);
        
        current_state = next_state;
    }

    printf("Final state: %s\n", state_names[current_state]);
}

int main(void) {
    int inputs[MAX_INPUTS];
    size_t count = 0;
    
    printf("Enter state machine inputs (0-2), one per line. Enter -1 to finish:\n");
    
    while (count < MAX_INPUTS) {
        int value;
        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Invalid input format\n");
            return EXIT_FAILURE;
        }
        
        if (value == -1) {
            break;
        }
        
        if (value < 0 || value >= INPUT_COUNT) {
            fprintf(stderr, "Input must be between 0 and %d\n", INPUT_COUNT - 1);
            return EXIT_FAILURE;
        }
        
        inputs[count] = value;
        count++;
    }
    
    if (count == 0) {
        fprintf(stderr, "No inputs provided\n");
        return EXIT_FAILURE;
    }
    
    process_state_machine(inputs, count);
    
    return EXIT_SUCCESS;
}