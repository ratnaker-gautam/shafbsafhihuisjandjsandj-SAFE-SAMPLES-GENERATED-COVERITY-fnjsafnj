//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 8
#define MAX_SYMBOLS 16

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
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
    {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_A},
    {STATE_E, '0', STATE_B}, {STATE_E, '1', STATE_C},
    {STATE_F, '0', STATE_D}, {STATE_F, '1', STATE_E},
    {STATE_G, '0', STATE_F}, {STATE_G, '1', STATE_G},
    {STATE_H, '0', STATE_H}, {STATE_H, '1', STATE_A}
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

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    return 1;
}

static void simulate_state_machine(const char* input) {
    state_t current_state = STATE_A;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        current_state = process_transition(current_state, input[i]);
        
        for (int j = 0; j < 1000; j++) {
            volatile int computation = j * j + j;
            (void)computation;
        }
    }
    
    const char* state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D",
        "STATE_E", "STATE_F", "STATE_G", "STATE_H"
    };
    
    if (current_state >= STATE_A && current_state <= STATE_H) {
        printf("Final state: %s\n", state_names[current_state]);
    } else {
        printf("Final state: UNKNOWN\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter binary sequence (max %d characters, only 0s and 1s): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input. Please enter only 0s and 1s (1-%d characters)\n", MAX_INPUT_LEN);
        return EXIT_FAILURE;
    }
    
    simulate_state_machine(input);
    
    return EXIT_SUCCESS;
}