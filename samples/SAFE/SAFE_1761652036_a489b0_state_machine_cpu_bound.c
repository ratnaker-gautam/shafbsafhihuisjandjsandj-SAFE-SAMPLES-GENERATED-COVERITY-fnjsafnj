//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
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

state_t transition_table[MAX_STATES][MAX_SYMBOLS] = {
    {STATE_B, STATE_C, STATE_D, STATE_E, STATE_F, STATE_G, STATE_H, STATE_I, STATE_J, STATE_A},
    {STATE_C, STATE_D, STATE_E, STATE_F, STATE_G, STATE_H, STATE_I, STATE_J, STATE_A, STATE_B},
    {STATE_D, STATE_E, STATE_F, STATE_G, STATE_H, STATE_I, STATE_J, STATE_A, STATE_B, STATE_C},
    {STATE_E, STATE_F, STATE_G, STATE_H, STATE_I, STATE_J, STATE_A, STATE_B, STATE_C, STATE_D},
    {STATE_F, STATE_G, STATE_H, STATE_I, STATE_J, STATE_A, STATE_B, STATE_C, STATE_D, STATE_E},
    {STATE_G, STATE_H, STATE_I, STATE_J, STATE_A, STATE_B, STATE_C, STATE_D, STATE_E, STATE_F},
    {STATE_H, STATE_I, STATE_J, STATE_A, STATE_B, STATE_C, STATE_D, STATE_E, STATE_F, STATE_G},
    {STATE_I, STATE_J, STATE_A, STATE_B, STATE_C, STATE_D, STATE_E, STATE_F, STATE_G, STATE_H},
    {STATE_J, STATE_A, STATE_B, STATE_C, STATE_D, STATE_E, STATE_F, STATE_G, STATE_H, STATE_I},
    {STATE_A, STATE_B, STATE_C, STATE_D, STATE_E, STATE_F, STATE_G, STATE_H, STATE_I, STATE_J}
};

symbol_t char_to_symbol(char c) {
    if (c >= '0' && c <= '9') {
        return (symbol_t)(c - '0');
    }
    return SYMBOL_0;
}

void process_state_machine(const char* input) {
    state_t current_state = STATE_A;
    size_t len = strlen(input);
    
    if (len == 0) {
        printf("Empty input sequence\n");
        return;
    }
    
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    printf("Initial state: A\n");
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)input[i])) {
            printf("Invalid character '%c' at position %zu\n", input[i], i);
            return;
        }
        
        symbol_t symbol = char_to_symbol(input[i]);
        state_t next_state = transition_table[current_state][symbol];
        
        printf("Input '%c' -> State ", input[i]);
        switch (next_state) {
            case STATE_A: printf("A"); break;
            case STATE_B: printf("B"); break;
            case STATE_C: printf("C"); break;
            case STATE_D: printf("D"); break;
            case STATE_E: printf("E"); break;
            case STATE_F: printf("F"); break;
            case STATE_G: printf("G"); break;
            case STATE_H: printf("H"); break;
            case STATE_I: printf("I"); break;
            case STATE_J: printf("J"); break;
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
        case STATE_F: printf("F"); break;
        case STATE_G: printf("G"); break;
        case STATE_H: printf("H"); break;
        case STATE_I: printf("I"); break;
        case STATE_J: printf("J"); break;
    }
    printf("\n");
}

int main(void) {
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
        printf("No input provided\n");
        return 1;
    }
    
    process_state_machine(input);
    
    return 0;
}