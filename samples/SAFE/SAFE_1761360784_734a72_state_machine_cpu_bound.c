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
} state_t;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_COUNT
} input_t;

static const state_t transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E"
};

static int process_state_machine(const input_t* inputs, size_t input_count) {
    state_t current_state = STATE_A;
    size_t transitions = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= INPUT_COUNT) {
            return -1;
        }
        
        state_t next_state = transition_table[current_state][inputs[i]];
        if (next_state >= STATE_COUNT) {
            return -1;
        }
        
        current_state = next_state;
        transitions++;
    }
    
    return (int)transitions;
}

static int generate_random_inputs(input_t* buffer, size_t count) {
    if (count > MAX_INPUTS || buffer == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < count; i++) {
        buffer[i] = (input_t)(rand() % INPUT_COUNT);
    }
    
    return 0;
}

static void print_state_sequence(const input_t* inputs, size_t input_count) {
    state_t current_state = STATE_A;
    printf("State sequence: %s", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= INPUT_COUNT) {
            printf(" [INVALID]");
            break;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        if (current_state >= STATE_COUNT) {
            printf(" [INVALID]");
            break;
        }
        
        printf(" -> %s", state_names[current_state]);
    }
    
    printf("\n");
}

int main(void) {
    const size_t input_count = 50;
    input_t inputs[MAX_INPUTS];
    
    if (input_count > MAX_INPUTS) {
        fprintf(stderr, "Input count exceeds maximum\n");
        return EXIT_FAILURE;
    }
    
    if (generate_random_inputs(inputs, input_count) != 0) {
        fprintf(stderr, "Failed to generate inputs\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing state machine with %zu inputs...\n", input_count);
    
    int result = process_state_machine(inputs, input_count);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Successfully processed %d transitions\n", result);
    print_state_sequence(inputs, input_count);
    
    state_t final_state = STATE_A;
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < INPUT_COUNT) {
            final_state = transition_table[final_state][inputs[i]];
        }
    }
    
    printf("Final state: %s\n", state_names[final_state]);
    
    return EXIT_SUCCESS;
}