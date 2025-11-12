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

static const state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static int process_state_machine(const input_t *inputs, size_t input_count) {
    if (inputs == NULL || input_count == 0 || input_count > MAX_INPUTS) {
        return -1;
    }
    
    state_t current_state = STATE_A;
    uint64_t computation_result = 1;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        for (int j = 0; j < 1000; j++) {
            computation_result = (computation_result * 6364136223846793005ULL) + 1;
            computation_result ^= computation_result >> 32;
        }
    }
    
    return (int)(computation_result % 1000);
}

static int parse_input(const char *input_str, input_t *inputs, size_t max_inputs) {
    if (input_str == NULL || inputs == NULL || max_inputs == 0) {
        return -1;
    }
    
    size_t input_count = 0;
    size_t len = strlen(input_str);
    
    if (len == 0 || len > max_inputs) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        switch (input_str[i]) {
            case 'x': case 'X':
                inputs[input_count++] = INPUT_X;
                break;
            case 'y': case 'Y':
                inputs[input_count++] = INPUT_Y;
                break;
            case 'z': case 'Z':
                inputs[input_count++] = INPUT_Z;
                break;
            default:
                return -1;
        }
        
        if (input_count >= max_inputs) {
            break;
        }
    }
    
    return (int)input_count;
}

int main(void) {
    char input_buffer[1024];
    input_t inputs[MAX_INPUTS];
    
    printf("Enter state machine inputs (x/y/z characters, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    int input_count = parse_input(input_buffer, inputs, MAX_INPUTS);
    if (input_count <= 0) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    int result = process_state_machine(inputs, (size_t)input_count);
    if (result < 0) {
        fprintf(stderr, "State machine processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine computation result: %d\n", result);
    return EXIT_SUCCESS;
}