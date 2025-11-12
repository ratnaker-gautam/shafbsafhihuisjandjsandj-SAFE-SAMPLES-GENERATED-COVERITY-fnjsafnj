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
    {1000000, 2000000, 500000},
    {1500000, 2500000, 750000},
    {2000000, 3000000, 1000000},
    {2500000, 3500000, 1250000},
    {3000000, 4000000, 1500000}
};

static void cpu_intensive_work(int iterations) {
    volatile double result = 1.0;
    for (int i = 0; i < iterations; i++) {
        result += (double)i / (i + 1.0);
        result *= 1.000001;
    }
}

static int validate_input_sequence(const input_t *inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return 0;
        }
    }
    return 1;
}

static input_t char_to_input(char c) {
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default: return -1;
    }
}

int main(void) {
    char input_buffer[256];
    input_t input_sequence[MAX_INPUTS];
    size_t input_count = 0;
    
    printf("Enter state machine inputs (x/y/z, max %d): ", MAX_INPUTS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < len && input_count < MAX_INPUTS; i++) {
        if (input_buffer[i] == ' ') continue;
        input_t input = char_to_input(input_buffer[i]);
        if (input == -1) {
            fprintf(stderr, "Invalid input character: %c\n", input_buffer[i]);
            return EXIT_FAILURE;
        }
        input_sequence[input_count++] = input;
    }
    
    if (!validate_input_sequence(input_sequence, input_count)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    state_t current_state = STATE_A;
    int total_iterations = 0;
    
    printf("Starting state machine simulation...\n");
    printf("Initial state: A\n");
    
    for (size_t i = 0; i < input_count; i++) {
        input_t current_input = input_sequence[i];
        int iterations = process_count_table[current_state][current_input];
        
        if (iterations < 0 || iterations > INT_MAX / 2) {
            fprintf(stderr, "Invalid iteration count\n");
            return EXIT_FAILURE;
        }
        
        cpu_intensive_work(iterations);
        
        if (total_iterations > INT_MAX - iterations) {
            fprintf(stderr, "Integer overflow detected\n");
            return EXIT_FAILURE;
        }
        total_iterations += iterations;
        
        state_t next_state = transition_table[current_state][current_input];
        
        printf("Input: ");
        switch (current_input) {
            case INPUT_X: printf("X"); break;
            case INPUT_Y: printf("Y"); break;
            case INPUT_Z: printf("Z"); break;
        }
        printf(" -> State: ");
        switch (next_state) {
            case STATE_A: printf("A"); break;
            case STATE_B: printf("B"); break;
            case STATE_C: printf("C"); break;
            case STATE_D: printf("D"); break;
            case STATE_E: printf("E"); break;
        }
        printf(" (Work: %d iterations)\n", iterations);
        
        current_state = next_state;
    }
    
    printf("Final state: ");
    switch (current_state) {
        case STATE_A: printf("A"); break;
        case STATE_B: printf("B"); break;
        case STATE_C: printf("C"); break;
        case STATE_D: printf("D"); break;
        case STATE_E: printf("E"); break;
    }
    printf("\nTotal work performed: %d iterations\n", total_iterations