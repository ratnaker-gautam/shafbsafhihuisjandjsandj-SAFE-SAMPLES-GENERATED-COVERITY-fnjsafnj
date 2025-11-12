//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_WHITESPACE,
    STATE_ERROR,
    STATE_END,
    STATE_COUNT
} state_t;

typedef state_t (*transition_func_t)(char);

state_t transition_start(char c) {
    if (isalpha(c) || c == '_') return STATE_IDENTIFIER;
    if (isdigit(c)) return STATE_NUMBER;
    if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
    if (isspace(c)) return STATE_WHITESPACE;
    if (c == '\0') return STATE_END;
    return STATE_ERROR;
}

state_t transition_identifier(char c) {
    if (isalnum(c) || c == '_') return STATE_IDENTIFIER;
    return transition_start(c);
}

state_t transition_number(char c) {
    if (isdigit(c)) return STATE_NUMBER;
    if (c == '.') return STATE_NUMBER;
    return transition_start(c);
}

state_t transition_operator(char c) {
    if (strchr("+-*/=<>!&|", c)) return STATE_OPERATOR;
    return transition_start(c);
}

state_t transition_whitespace(char c) {
    if (isspace(c)) return STATE_WHITESPACE;
    return transition_start(c);
}

state_t transition_error(char c) {
    return STATE_ERROR;
}

state_t transition_end(char c) {
    return STATE_END;
}

transition_func_t transitions[STATE_COUNT] = {
    transition_start,
    transition_identifier,
    transition_number,
    transition_operator,
    transition_whitespace,
    transition_error,
    transition_end
};

void process_input(const char *input) {
    state_t current_state = STATE_START;
    const char *ptr = input;
    const char *token_start = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char current_char = *ptr;
        state_t next_state = transitions[current_state](current_char);
        
        if (current_state != next_state) {
            size_t token_length = ptr - token_start;
            if (token_length > 0 && current_state != STATE_WHITESPACE) {
                printf("Token: ");
                for (const char *p = token_start; p != ptr; ++p) {
                    putchar(*p);
                }
                printf(" (State: %d)\n", current_state);
            }
            token_start = ptr;
        }
        
        current_state = next_state;
        
        if (current_char == '\0') break;
        ++ptr;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input at position %ld\n", ptr - input);
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter input to tokenize (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        fprintf(stderr, "Error: Empty input\n");
        return EXIT_FAILURE;
    }
    
    process_input(input);
    
    return EXIT_SUCCESS;
}