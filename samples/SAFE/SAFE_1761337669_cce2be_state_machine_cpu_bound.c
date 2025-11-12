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
    {STATE_F, SYMBOL_0, STATE_B}, {STATE_F, SYMBOL_1, STATE_C},
    {STATE_G, SYMBOL_2, STATE_D}, {STATE_G, SYMBOL_3, STATE_E},
    {STATE_H, SYMBOL_4, STATE_F}, {STATE_H, SYMBOL_5, STATE_G},
    {STATE_I, SYMBOL_6, STATE_H}, {STATE_I, SYMBOL_7, STATE_I},
    {STATE_J, SYMBOL_8, STATE_J}, {STATE_J, SYMBOL_9, STATE_A}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E",
    "STATE_F", "STATE_G", "STATE_H", "STATE_I", "STATE_J"
};

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(input[i])) return 0;
    }
    return 1;
}

static symbol_t char_to_symbol(char c) {
    if (c >= '0' && c <= '9') {
        return (symbol_t)(c - '0');
    }
    return SYMBOL_0;
}

static state_t process_transition(state_t current, symbol_t symbol) {
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_symbol == symbol) {
            return transitions[i].next_state;
        }
    }
    return current;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter sequence of digits (0-9): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: must contain only digits 0-9, max %d characters\n", MAX_INPUT_LEN);
        return 1;
    }
    
    state_t current_state = STATE_A;
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < len; i++) {
        symbol_t symbol = char_to_symbol(input[i]);
        state_t next_state = process_transition(current_state, symbol);
        printf("Input: %c, Transition: %s -> %s\n", 
               input[i], state_names[current_state], state_names[next_state]);
        current_state = next_state;
    }
    
    printf("Final state: %s\n", state_names[current_state]);
    
    uint64_t computation = 0;
    for (int i = 0; i < 1000000; i++) {
        for (size_t j = 0; j < len; j++) {
            computation += (uint64_t)input[j] * (i + 1);
            computation ^= (computation << 13);
            computation ^= (computation >> 7);
            computation ^= (computation << 17);
        }
    }
    
    printf("Computation checksum: %lu\n", computation);
    
    return 0;
}