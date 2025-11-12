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
    {STATE_B, SYMBOL_0, STATE_D}, {STATE_B, SYMBOL_1, STATE_E},
    {STATE_C, SYMBOL_0, STATE_F}, {STATE_C, SYMBOL_1, STATE_G},
    {STATE_D, SYMBOL_0, STATE_H}, {STATE_D, SYMBOL_1, STATE_I},
    {STATE_E, SYMBOL_0, STATE_J}, {STATE_E, SYMBOL_1, STATE_A},
    {STATE_F, SYMBOL_0, STATE_B}, {STATE_F, SYMBOL_1, STATE_C},
    {STATE_G, SYMBOL_0, STATE_D}, {STATE_G, SYMBOL_1, STATE_E},
    {STATE_H, SYMBOL_0, STATE_F}, {STATE_H, SYMBOL_1, STATE_G},
    {STATE_I, SYMBOL_0, STATE_H}, {STATE_I, SYMBOL_1, STATE_I},
    {STATE_J, SYMBOL_0, STATE_J}, {STATE_J, SYMBOL_1, STATE_A}
};

static const char* state_names[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J"
};

static const char* symbol_names[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

static state_t find_next_state(state_t current, symbol_t symbol) {
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_symbol == symbol) {
            return transitions[i].next_state;
        }
    }
    return STATE_A;
}

static int is_valid_digit(char c) {
    return c >= '0' && c <= '9';
}

static symbol_t char_to_symbol(char c) {
    if (!is_valid_digit(c)) {
        return SYMBOL_0;
    }
    return (symbol_t)(c - '0');
}

static void process_state_machine(const char* input) {
    state_t current_state = STATE_A;
    size_t len = strlen(input);
    
    printf("Initial state: %s\n", state_names[current_state]);
    printf("Input sequence: %s\n", input);
    printf("State transitions:\n");
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_digit(input[i])) {
            printf("Invalid input symbol '%c' at position %zu\n", input[i], i);
            return;
        }
        
        symbol_t symbol = char_to_symbol(input[i]);
        state_t next_state = find_next_state(current_state, symbol);
        
        printf("  State %s + Symbol %s -> State %s\n",
               state_names[current_state],
               symbol_names[symbol],
               state_names[next_state]);
        
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a sequence of digits (0-9): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_digit(input[i])) {
            fprintf(stderr, "Invalid character '%c' at position %zu\n", input[i], i);
            return 1;
        }
    }
    
    process_state_machine(input);
    
    return 0;
}