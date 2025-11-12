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

static const char* state_name(state_t state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    input_t inputs[MAX_INPUTS];
    int input_count = 0;
    
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
        if (input_buffer[i] != ' ' && input_buffer[i] != '\t') {
            inputs[input_count++] = parse_input(input_buffer[i]);
        }
    }
    
    if (input_count == 0) {
        fprintf(stderr, "No valid inputs found\n");
        return EXIT_FAILURE;
    }
    
    state_t current_state = STATE_A;
    printf("Initial state: %s\n", state_name(current_state));
    
    for (int i = 0; i < input_count; i++) {
        input_t input = inputs[i];
        if (input < 0 || input > 2) {
            fprintf(stderr, "Invalid input value\n");
            return EXIT_FAILURE;
        }
        
        if (current_state < 0 || current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid current state\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = transition_table[current_state][input];
        int iterations = process_count_table[current_state][input];
        
        printf("Input: %c -> State: %s -> ", 
               input == INPUT_X ? 'X' : (input == INPUT_Y ? 'Y' : 'Z'),
               state_name(next_state));
        
        cpu_intensive_work(iterations);
        
        printf("Processed %d iterations\n", iterations);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_name(current_state));
    printf("Total inputs processed: %d\n", input_count);
    
    return EXIT_SUCCESS;
}