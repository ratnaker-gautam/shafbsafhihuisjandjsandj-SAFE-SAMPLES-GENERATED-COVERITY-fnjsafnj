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
    INPUT_0,
    INPUT_1,
    INPUT_INVALID
} input_t;

static const state_t transition_table[STATE_COUNT][2] = {
    {STATE_B, STATE_C},
    {STATE_D, STATE_A},
    {STATE_E, STATE_B},
    {STATE_A, STATE_D},
    {STATE_C, STATE_E}
};

static input_t parse_input(char c) {
    if (c == '0') return INPUT_0;
    if (c == '1') return INPUT_1;
    return INPUT_INVALID;
}

static void process_state_machine(const char* input_sequence) {
    state_t current_state = STATE_A;
    size_t input_len = strlen(input_sequence);
    
    if (input_len == 0) {
        printf("Empty input sequence\n");
        return;
    }
    
    if (input_len > MAX_INPUTS) {
        printf("Input sequence too long\n");
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        input_t input = parse_input(input_sequence[i]);
        
        if (input == INPUT_INVALID) {
            printf("Invalid input character at position %zu\n", i);
            return;
        }
        
        current_state = transition_table[current_state][input];
        
        printf("Step %zu: Input '%c' -> State %d\n", 
               i + 1, input_sequence[i], current_state);
    }
    
    printf("Final state: %d\n", current_state);
}

static int validate_input_string(const char* str) {
    if (str == NULL) return 0;
    
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1') return 0;
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUTS + 2];
    
    printf("Enter binary input sequence (max %d characters, only 0s and 1s): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input_string(input_buffer)) {
        printf("Invalid input: must contain only 0s and 1s, 1-%d characters\n", MAX_INPUTS);
        return EXIT_FAILURE;
    }
    
    process_state_machine(input_buffer);
    
    return EXIT_SUCCESS;
}