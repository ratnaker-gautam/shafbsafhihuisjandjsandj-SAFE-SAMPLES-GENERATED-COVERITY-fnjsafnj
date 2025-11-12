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

typedef enum {
    SYMBOL_0,
    SYMBOL_1,
    SYMBOL_2,
    SYMBOL_3,
    SYMBOL_4,
    SYMBOL_5,
    SYMBOL_6,
    SYMBOL_7,
    SYMBOL_8,
    SYMBOL_9
} symbol_t;

typedef struct {
    state_t current_state;
    symbol_t input_symbol;
    state_t next_state;
} transition_t;

static transition_t transitions[] = {
    {STATE_A, SYMBOL_0, STATE_B}, {STATE_A, SYMBOL_1, STATE_C},
    {STATE_B, SYMBOL_2, STATE_D}, {STATE_B, SYMBOL_3, STATE_E},
    {STATE_C, SYMBOL_4, STATE_F}, {STATE_C, SYMBOL_5, STATE_G},
    {STATE_D, SYMBOL_6, STATE_H}, {STATE_D, SYMBOL_7, STATE_I},
    {STATE_E, SYMBOL_8, STATE_J}, {STATE_E, SYMBOL_9, STATE_A},
    {STATE_F, SYMBOL_0, STATE_C}, {STATE_F, SYMBOL_1, STATE_D},
    {STATE_G, SYMBOL_2, STATE_E}, {STATE_G, SYMBOL_3, STATE_F},
    {STATE_H, SYMBOL_4, STATE_G}, {STATE_H, SYMBOL_5, STATE_H},
    {STATE_I, SYMBOL_6, STATE_I}, {STATE_I, SYMBOL_7, STATE_J},
    {STATE_J, SYMBOL_8, STATE_A}, {STATE_J, SYMBOL_9, STATE_B}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E",
    "STATE_F", "STATE_G", "STATE_H", "STATE_I", "STATE_J"
};

static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

symbol_t char_to_symbol(char c) {
    if (c >= '0' && c <= '9') {
        return (symbol_t)(c - '0');
    }
    return SYMBOL_0;
}

state_t process_transition(state_t current, symbol_t symbol) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_symbol == symbol) {
            return transitions[i].next_state;
        }
    }
    return current;
}

void simulate_state_machine(const char* input) {
    state_t current_state = STATE_A;
    int input_len = strlen(input);
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    printf("Initial state: %s\n", state_names[current_state]);
    printf("Input sequence: %s\n", input);
    printf("State transitions:\n");
    
    for (int i = 0; i < input_len; i++) {
        if (!isdigit((unsigned char)input[i])) {
            printf("Invalid input character: %c\n", input[i]);
            return;
        }
        
        symbol_t symbol = char_to_symbol(input[i]);
        state_t next_state = process_transition(current_state, symbol);
        
        printf("  %s --%c--> %s\n", 
               state_names[current_state], 
               input[i], 
               state_names[next_state]);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a sequence of digits (0-9): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    simulate_state_machine(input);
    
    return 0;
}