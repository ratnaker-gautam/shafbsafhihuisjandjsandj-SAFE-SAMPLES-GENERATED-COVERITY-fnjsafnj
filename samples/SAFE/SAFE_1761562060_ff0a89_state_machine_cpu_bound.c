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
    INPUT_INVALID
} input_t;

static const state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static input_t parse_input(char c) {
    if (c == '0') return INPUT_0;
    if (c == '1') return INPUT_1;
    if (c == '2') return INPUT_2;
    return INPUT_INVALID;
}

static void process_state_machine(const char* input_sequence) {
    state_t current_state = STATE_A;
    size_t input_len = strlen(input_sequence);
    
    if (input_len == 0 || input_len > MAX_INPUTS) {
        fprintf(stderr, "Error: Invalid input length\n");
        return;
    }
    
    printf("Initial state: A\n");
    
    for (size_t i = 0; i < input_len; i++) {
        input_t input = parse_input(input_sequence[i]);
        if (input == INPUT_INVALID) {
            fprintf(stderr, "Error: Invalid input character '%c'\n", input_sequence[i]);
            return;
        }
        
        state_t next_state = transition_table[current_state][input];
        
        printf("Input: %c | State: ", input_sequence[i]);
        switch (current_state) {
            case STATE_A: printf("A"); break;
            case STATE_B: printf("B"); break;
            case STATE_C: printf("C"); break;
            case STATE_D: printf("D"); break;
            case STATE_E: printf("E"); break;
        }
        printf(" -> ");
        switch (next_state) {
            case STATE_A: printf("A"); break;
            case STATE_B: printf("B"); break;
            case STATE_C: printf("C"); break;
            case STATE_D: printf("D"); break;
            case STATE_E: printf("E"); break;
        }
        printf("\n");
        
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
    printf("\n");
}

static int validate_input_string(const char* str) {
    if (str == NULL) return 0;
    
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2') {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUTS + 2];
    
    printf("Enter input sequence (0,1,2 only, max %d characters): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    } else if (len >= MAX_INPUTS) {
        fprintf(stderr, "Error: Input too long\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input_string(input_buffer)) {
        fprintf(stderr, "Error: Invalid input characters or length\n");
        return EXIT_FAILURE;
    }
    
    process_state_machine(input_buffer);
    
    return EXIT_SUCCESS;
}