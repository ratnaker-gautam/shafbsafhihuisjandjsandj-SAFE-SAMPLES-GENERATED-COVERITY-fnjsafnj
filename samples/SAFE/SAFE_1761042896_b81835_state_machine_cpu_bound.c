//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 8
#define MAX_SYMBOLS 8

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef enum {
    SYMBOL_0,
    SYMBOL_1,
    SYMBOL_2,
    SYMBOL_3,
    SYMBOL_4,
    SYMBOL_5,
    SYMBOL_6,
    SYMBOL_7
} Symbol;

typedef struct {
    State next_state;
    int output;
} Transition;

static Transition state_table[MAX_STATES][MAX_SYMBOLS];

void initialize_state_machine(void) {
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            state_table[i][j].next_state = (State)((i + j) % MAX_STATES);
            state_table[i][j].output = (i * j) % 100;
        }
    }
}

int validate_symbol(char c) {
    if (c >= '0' && c <= '7') {
        return 1;
    }
    return 0;
}

Symbol char_to_symbol(char c) {
    return (Symbol)(c - '0');
}

int process_input_sequence(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) {
        return -1;
    }
    
    State current_state = STATE_A;
    int total_output = 0;
    int iterations = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!validate_symbol(input[i])) {
            return -1;
        }
        
        Symbol symbol = char_to_symbol(input[i]);
        Transition trans = state_table[current_state][symbol];
        
        current_state = trans.next_state;
        total_output += trans.output;
        iterations++;
        
        if (iterations > 1000000) {
            return -1;
        }
    }
    
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            total_output = (total_output + i * j) % 10000;
        }
    }
    
    return total_output;
}

int main(void) {
    initialize_state_machine();
    
    char input[MAX_INPUT_LEN + 1];
    printf("Enter sequence of digits (0-7): ");
    
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
    
    int result = process_input_sequence(input, len);
    if (result == -1) {
        fprintf(stderr, "Invalid input or processing error\n");
        return 1;
    }
    
    printf("Final result: %d\n", result);
    return 0;
}