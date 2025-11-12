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
    INPUT_2,
    INPUT_INVALID
} input_t;

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static const char* state_names[STATE_COUNT] = {
    "A", "B", "C", "D", "E"
};

input_t parse_input(char c) {
    if (c == '0') return INPUT_0;
    if (c == '1') return INPUT_1;
    if (c == '2') return INPUT_2;
    return INPUT_INVALID;
}

void process_state_machine(const char* input_sequence, size_t length) {
    if (input_sequence == NULL || length == 0 || length > MAX_INPUTS) {
        fprintf(stderr, "Invalid input sequence\n");
        return;
    }
    
    state_t current_state = STATE_A;
    size_t valid_transitions = 0;
    
    for (size_t i = 0; i < length; i++) {
        input_t input = parse_input(input_sequence[i]);
        if (input == INPUT_INVALID) {
            fprintf(stderr, "Invalid input character at position %zu\n", i);
            continue;
        }
        
        state_t next_state = transition_table[current_state][input];
        printf("State %s + Input %c -> State %s\n", 
               state_names[current_state], 
               input_sequence[i],
               state_names[next_state]);
        
        current_state = next_state;
        valid_transitions++;
        
        if (valid_transitions > 1000000) {
            fprintf(stderr, "Too many transitions, stopping\n");
            break;
        }
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    printf("Total valid transitions: %zu\n", valid_transitions);
}

char* generate_test_sequence(size_t length) {
    if (length == 0 || length > MAX_INPUTS) {
        return NULL;
    }
    
    char* sequence = malloc(length + 1);
    if (sequence == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < length; i++) {
        int r = rand() % 3;
        sequence[i] = '0' + r;
    }
    sequence[length] = '\0';
    
    return sequence;
}

int main(void) {
    const size_t test_length = 50;
    
    char* test_input = generate_test_sequence(test_length);
    if (test_input == NULL) {
        fprintf(stderr, "Failed to generate test sequence\n");
        return EXIT_FAILURE;
    }
    
    printf("Testing state machine with sequence: %s\n", test_input);
    process_state_machine(test_input, test_length);
    
    free(test_input);
    
    return EXIT_SUCCESS;
}