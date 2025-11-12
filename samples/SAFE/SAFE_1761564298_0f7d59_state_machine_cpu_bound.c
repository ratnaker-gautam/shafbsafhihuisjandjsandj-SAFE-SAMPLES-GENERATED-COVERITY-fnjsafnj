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
    {100, 200, 50},
    {150, 250, 75},
    {200, 300, 100},
    {250, 350, 125},
    {300, 400, 150}
};

static void cpu_intensive_work(int iterations) {
    volatile double result = 1.0;
    for (int i = 0; i < iterations; i++) {
        result = result * 1.000001 + 0.000001;
    }
}

static input_t parse_input(char c) {
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default: return INPUT_X;
    }
}

static int validate_input_sequence(const char* sequence, size_t length) {
    if (sequence == NULL) return 0;
    if (length > MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < length; i++) {
        char c = sequence[i];
        if (c != 'x' && c != 'X' && c != 'y' && c != 'Y' && c != 'z' && c != 'Z') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input_buffer[1024];
    char processed_sequence[MAX_INPUTS + 1];
    size_t sequence_length = 0;
    
    printf("Enter state machine input sequence (x/y/z characters, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input_sequence(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input sequence\n");
        return EXIT_FAILURE;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input sequence\n");
        return EXIT_FAILURE;
    }
    
    sequence_length = input_len;
    memcpy(processed_sequence, input_buffer, sequence_length);
    processed_sequence[sequence_length] = '\0';
    
    state_t current_state = STATE_A;
    int total_work = 0;
    
    printf("Initial state: A\n");
    printf("Processing sequence: %s\n", processed_sequence);
    
    for (size_t i = 0; i < sequence_length; i++) {
        input_t input = parse_input(processed_sequence[i]);
        int work_iterations = process_count_table[current_state][input];
        
        printf("State %c, Input %c -> ", 'A' + current_state, processed_sequence[i]);
        
        cpu_intensive_work(work_iterations);
        total_work += work_iterations;
        
        state_t next_state = transition_table[current_state][input];
        printf("State %c (Work: %d)\n", 'A' + next_state, work_iterations);
        
        current_state = next_state;
    }
    
    printf("Final state: %c\n", 'A' + current_state);
    printf("Total work iterations: %d\n", total_work);
    
    return EXIT_SUCCESS;
}