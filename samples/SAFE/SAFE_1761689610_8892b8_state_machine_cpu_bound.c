//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_SYMBOLS 10

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H,
    STATE_I,
    STATE_J
} state_t;

typedef struct {
    state_t current_state;
    char input_symbol;
    state_t next_state;
} transition_t;

static transition_t transitions[] = {
    {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
    {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
    {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
    {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_I},
    {STATE_E, '0', STATE_J}, {STATE_E, '1', STATE_A},
    {STATE_F, '0', STATE_B}, {STATE_F, '1', STATE_C},
    {STATE_G, '0', STATE_D}, {STATE_G, '1', STATE_E},
    {STATE_H, '0', STATE_F}, {STATE_H, '1', STATE_G},
    {STATE_I, '0', STATE_H}, {STATE_I, '1', STATE_I},
    {STATE_J, '0', STATE_J}, {STATE_J, '1', STATE_A}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static state_t process_transition(state_t current, char symbol) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_symbol == symbol) {
            return transitions[i].next_state;
        }
    }
    return current;
}

static int is_valid_symbol(char c) {
    return c == '0' || c == '1';
}

static void process_input_string(const char* input) {
    state_t current_state = STATE_A;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_symbol(input[i])) {
            printf("Invalid input symbol '%c' at position %zu\n", input[i], i);
            return;
        }
        
        state_t next_state = process_transition(current_state, input[i]);
        
        for (int j = 0; j < 1000; j++) {
            volatile int computation = j * j + j;
            (void)computation;
        }
        
        current_state = next_state;
    }
    
    printf("Final state: %d\n", (int)current_state);
    printf("Input processed successfully: %s\n", input);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter binary string (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input string\n");
        return EXIT_FAILURE;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    process_input_string(input);
    
    return EXIT_SUCCESS;
}