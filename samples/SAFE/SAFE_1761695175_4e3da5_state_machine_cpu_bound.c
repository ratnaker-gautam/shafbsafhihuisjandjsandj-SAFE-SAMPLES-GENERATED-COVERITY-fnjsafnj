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
} State;

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
} Symbol;

typedef struct {
    State current_state;
    Symbol input_symbol;
    State next_state;
} Transition;

static const Transition transitions[] = {
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

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

Symbol char_to_symbol(char c) {
    if (c >= '0' && c <= '9') {
        return (Symbol)(c - '0');
    }
    return SYMBOL_0;
}

State process_state_machine(const char* input, size_t len) {
    State current = STATE_A;
    
    for (size_t i = 0; i < len; i++) {
        Symbol sym = char_to_symbol(input[i]);
        int transition_found = 0;
        
        for (size_t j = 0; j < num_transitions; j++) {
            if (transitions[j].current_state == current && 
                transitions[j].input_symbol == sym) {
                current = transitions[j].next_state;
                transition_found = 1;
                break;
            }
        }
        
        if (!transition_found) {
            return STATE_A;
        }
    }
    
    return current;
}

void validate_input(const char* input, size_t* valid_len) {
    *valid_len = 0;
    if (!input) return;
    
    size_t len = 0;
    while (input[len] != '\0' && len < MAX_INPUT_LEN) {
        if (!isdigit((unsigned char)input[len])) {
            break;
        }
        len++;
    }
    
    *valid_len = len;
}

void perform_cpu_work(State final_state) {
    volatile uint64_t counter = 0;
    uint64_t iterations = (final_state + 1) * 1000000ULL;
    
    for (uint64_t i = 0; i < iterations; i++) {
        counter += i;
        if (counter > 1000000000ULL) {
            counter = 0;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter digit sequence (max %d digits): ", MAX_INPUT_LEN);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len;
    validate_input(input_buffer, &input_len);
    
    if (input_len == 0) {
        fprintf(stderr, "Invalid input: must contain only digits 0-9\n");
        return 1;
    }
    
    printf("Processing %zu digits through state machine...\n", input_len);
    
    State final_state = process_state_machine(input_buffer, input_len);
    
    printf("Final state: %d\n", (int)final_state);
    printf("Performing CPU-intensive work based on final state...\n");
    
    perform_cpu_work(final_state);
    
    printf("Processing complete.\n");
    
    return 0;
}