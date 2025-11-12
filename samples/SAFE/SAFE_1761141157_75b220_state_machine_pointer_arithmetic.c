//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 8
#define MAX_TRANSITIONS 16

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_WHITESPACE,
    STATE_ERROR,
    STATE_END,
    STATE_UNKNOWN
} State;

typedef struct {
    State current;
    char trigger;
    State next;
} Transition;

Transition transitions[MAX_TRANSITIONS] = {
    {STATE_START, ' ', STATE_WHITESPACE},
    {STATE_START, '\t', STATE_WHITESPACE},
    {STATE_START, '\n', STATE_WHITESPACE},
    {STATE_START, '+', STATE_OPERATOR},
    {STATE_START, '-', STATE_OPERATOR},
    {STATE_START, '*', STATE_OPERATOR},
    {STATE_START, '/', STATE_OPERATOR},
    {STATE_START, '=', STATE_OPERATOR},
    {STATE_START, '0', STATE_NUMBER},
    {STATE_START, '1', STATE_NUMBER},
    {STATE_START, '2', STATE_NUMBER},
    {STATE_START, '3', STATE_NUMBER},
    {STATE_START, '4', STATE_NUMBER},
    {STATE_START, '5', STATE_NUMBER},
    {STATE_START, '6', STATE_NUMBER},
    {STATE_START, '7', STATE_NUMBER}
};

const char* state_names[MAX_STATES] = {
    "START", "IDENTIFIER", "NUMBER", "OPERATOR",
    "WHITESPACE", "ERROR", "END", "UNKNOWN"
};

State find_next_state(State current, char input) {
    for (size_t i = 0; i < MAX_TRANSITIONS; i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    
    if (current == STATE_START) {
        if (isalpha(input)) return STATE_IDENTIFIER;
        if (isdigit(input)) return STATE_NUMBER;
    }
    
    if (current == STATE_IDENTIFIER && (isalnum(input) || input == '_')) {
        return STATE_IDENTIFIER;
    }
    
    if (current == STATE_NUMBER && isdigit(input)) {
        return STATE_NUMBER;
    }
    
    if (current == STATE_WHITESPACE && isspace(input)) {
        return STATE_WHITESPACE;
    }
    
    if (current == STATE_OPERATOR && strchr("+-*/=", input) != NULL) {
        return STATE_OPERATOR;
    }
    
    return STATE_UNKNOWN;
}

void process_input(const char* input) {
    if (input == NULL) return;
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len == 0 || len >= MAX_INPUT_LEN) return;
    
    State current_state = STATE_START;
    const char* ptr = input;
    const char* token_start = ptr;
    
    printf("Input: %s\n", input);
    printf("Token analysis:\n");
    
    while (*ptr != '\0' && (size_t)(ptr - input) < len) {
        State next_state = find_next_state(current_state, *ptr);
        
        if (next_state != current_state && current_state != STATE_START) {
            size_t token_len = ptr - token_start;
            if (token_len > 0 && token_len < MAX_INPUT_LEN) {
                printf("  %.*s -> %s\n", (int)token_len, token_start, state_names[current_state]);
            }
            token_start = ptr;
        }
        
        if (next_state == STATE_UNKNOWN) {
            printf("  %c -> ERROR\n", *ptr);
            token_start = ptr + 1;
            next_state = STATE_START;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    size_t final_token_len = ptr - token_start;
    if (final_token_len > 0 && final_token_len < MAX_INPUT_LEN && current_state != STATE_START) {
        printf("  %.*s -> %s\n", (int)final_token_len, token_start, state_names[current_state]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter expression to analyze (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    process_input(input);
    return EXIT_SUCCESS;
}