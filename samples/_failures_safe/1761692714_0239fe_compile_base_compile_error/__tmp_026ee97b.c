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

typedef enum {
    SYMBOL_DIGIT,
    SYMBOL_LETTER,
    SYMBOL_SPACE,
    SYMBOL_PUNCT,
    SYMBOL_OTHER
} symbol_t;

typedef struct {
    state_t current_state;
    symbol_t input_symbol;
    state_t next_state;
    uint32_t counter_increment;
} transition_t;

static const transition_t transitions[] = {
    {STATE_A, SYMBOL_DIGIT, STATE_B, 1},
    {STATE_A, SYMBOL_LETTER, STATE_C, 1},
    {STATE_A, SYMBOL_SPACE, STATE_A, 0},
    {STATE_B, SYMBOL_DIGIT, STATE_B, 1},
    {STATE_B, SYMBOL_LETTER, STATE_D, 2},
    {STATE_B, SYMBOL_SPACE, STATE_A, 0},
    {STATE_C, SYMBOL_DIGIT, STATE_D, 2},
    {STATE_C, SYMBOL_LETTER, STATE_C, 1},
    {STATE_C, SYMBOL_SPACE, STATE_A, 0},
    {STATE_D, SYMBOL_DIGIT, STATE_E, 3},
    {STATE_D, SYMBOL_LETTER, STATE_F, 3},
    {STATE_D, SYMBOL_SPACE, STATE_A, 0},
    {STATE_E, SYMBOL_DIGIT, STATE_E, 3},
    {STATE_E, SYMBOL_LETTER, STATE_G, 4},
    {STATE_E, SYMBOL_SPACE, STATE_A, 0},
    {STATE_F, SYMBOL_DIGIT, STATE_G, 4},
    {STATE_F, SYMBOL_LETTER, STATE_F, 3},
    {STATE_F, SYMBOL_SPACE, STATE_A, 0},
    {STATE_G, SYMBOL_DIGIT, STATE_H, 5},
    {STATE_G, SYMBOL_LETTER, STATE_H, 5},
    {STATE_G, SYMBOL_SPACE, STATE_A, 0},
    {STATE_H, SYMBOL_DIGIT, STATE_H, 5},
    {STATE_H, SYMBOL_LETTER, STATE_H, 5},
    {STATE_H, SYMBOL_SPACE, STATE_A, 0}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static symbol_t classify_char(char c) {
    if (isdigit((unsigned char)c)) return SYMBOL_DIGIT;
    if (isalpha((unsigned char)c)) return SYMBOL_LETTER;
    if (isspace((unsigned char)c)) return SYMBOL_SPACE;
    if (ispunct((unsigned char)c)) return SYMBOL_PUNCT;
    return SYMBOL_OTHER;
}

static state_t find_next_state(state_t current, symbol_t symbol) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_symbol == symbol) {
            return transitions[i].next_state;
        }
    }
    return STATE_A;
}

static uint32_t find_counter_increment(state_t current, symbol_t symbol) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_symbol == symbol) {
            return transitions[i].counter_increment;
        }
    }
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    uint32_t total_count = 0;
    state_t current_state = STATE_A;
    
    printf("Enter text to process: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        symbol_t symbol = classify_char(input[i]);
        uint32_t increment = find_counter_increment(current_state, symbol);
        
        if (total_count > UINT32_MAX - increment) {
            fprintf(stderr, "Counter overflow detected\n");
            return 1;
        }
        
        total_count += increment;
        current_state = find_next_state(current_state, symbol);
        
        for (int j = 0; j < 1000; j++) {
            total_count = (total_count * 1103515245u + 12345u) & 0x7FFFFFFF;
        }
    }
    
    printf("Final state: %d