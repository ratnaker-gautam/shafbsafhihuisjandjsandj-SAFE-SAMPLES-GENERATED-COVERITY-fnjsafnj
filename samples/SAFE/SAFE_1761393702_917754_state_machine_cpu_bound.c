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

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static int process_count_table[STATE_COUNT][3] = {
    {10, 20, 5},
    {15, 25, 8},
    {12, 18, 6},
    {22, 30, 10},
    {18, 24, 7}
};

static void validate_input_sequence(const input_t *inputs, size_t count) {
    if (inputs == NULL) {
        fprintf(stderr, "Error: Input sequence is NULL\n");
        exit(EXIT_FAILURE);
    }
    if (count == 0 || count > MAX_INPUTS) {
        fprintf(stderr, "Error: Invalid input sequence length\n");
        exit(EXIT_FAILURE);
    }
}

static input_t parse_input_char(char c) {
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default:
            fprintf(stderr, "Error: Invalid input character '%c'\n", c);
            exit(EXIT_FAILURE);
    }
}

static void cpu_intensive_work(int iterations) {
    if (iterations < 0) {
        fprintf(stderr, "Error: Invalid iteration count\n");
        exit(EXIT_FAILURE);
    }
    
    volatile double result = 1.0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 100; j++) {
            result = result * 1.0001 + 0.0001;
        }
    }
}

static void run_state_machine(const input_t *inputs, size_t count) {
    state_t current_state = STATE_A;
    int total_work = 0;
    
    for (size_t i = 0; i < count; i++) {
        input_t input = inputs[i];
        if (input < INPUT_X || input > INPUT_Z) {
            fprintf(stderr, "Error: Invalid input value\n");
            exit(EXIT_FAILURE);
        }
        
        int work_iterations = process_count_table[current_state][input];
        if (work_iterations < 0) {
            fprintf(stderr, "Error: Invalid work iteration count\n");
            exit(EXIT_FAILURE);
        }
        
        cpu_intensive_work(work_iterations);
        total_work += work_iterations;
        
        state_t next_state = transition_table[current_state][input];
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            fprintf(stderr, "Error: Invalid state transition\n");
            exit(EXIT_FAILURE);
        }
        
        printf("State: %d, Input: %d, Work: %d, Next: %d\n", 
               current_state, input, work_iterations, next_state);
        
        current_state = next_state;
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total work iterations: %d\n", total_work);
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter input sequence (x/y/z characters, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0 || input_len > MAX_INPUTS) {
        fprintf(stderr, "Error: Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    input_t *inputs = malloc(input_len * sizeof(input_t));
    if (inputs == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        inputs[i] = parse_input_char(input_buffer[i]);
    }
    
    validate_input_sequence(inputs, input_len);
    run_state_machine(inputs, input_len);
    
    free(inputs);
    return EXIT_SUCCESS;
}