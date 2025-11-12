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
    if (isalpha(c)) return STATE_IDENTIFIER;
    if (isdigit(c)) return STATE_NUMBER;
    if (strchr("+-*/", c)) return STATE_OPERATOR;
    if (isspace(c)) return STATE_WHITESPACE;
    if (c == '\0') return STATE_END;
    return STATE_ERROR;
}

state_t transition_identifier(char c) {
    if (isalnum(c)) return STATE_IDENTIFIER;
    if (isspace(c)) return STATE_WHITESPACE;
    if (strchr("+-*/", c)) return STATE_OPERATOR;
    if (c == '\0') return STATE_END;
    return STATE_ERROR;
}

state_t transition_number(char c) {
    if (isdigit(c)) return STATE_NUMBER;
    if (isspace(c)) return STATE_WHITESPACE;
    if (strchr("+-*/", c)) return STATE_OPERATOR;
    if (c == '\0') return STATE_END;
    return STATE_ERROR;
}

state_t transition_operator(char c) {
    if (isalpha(c)) return STATE_IDENTIFIER;
    if (isdigit(c)) return STATE_NUMBER;
    if (isspace(c)) return STATE_WHITESPACE;
    if (c == '\0') return STATE_END;
    return STATE_ERROR;
}

state_t transition_whitespace(char c) {
    if (isalpha(c)) return STATE_IDENTIFIER;
    if (isdigit(c)) return STATE_NUMBER;
    if (strchr("+-*/", c)) return STATE_OPERATOR;
    if (isspace(c)) return STATE_WHITESPACE;
    if (c == '\0') return STATE_END;
    return STATE_ERROR;
}

state_t transition_error(char c) {
    return STATE_ERROR;
}

state_t transition_end(char c) {
    return STATE_END;
}

int main(void) {
    transition_func_t transitions[STATE_COUNT] = {
        transition_start,
        transition_identifier,
        transition_number,
        transition_operator,
        transition_whitespace,
        transition_error,
        transition_end
    };
    
    char input[MAX_INPUT + 1] = {0};
    char *input_ptr = input;
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    state_t current_state = STATE_START;
    char *token_start = input_ptr;
    int token_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char current_char = *input_ptr;
        state_t next_state = transitions[current_state](current_char);
        
        if (next_state != current_state) {
            if (current_state != STATE_START && current_state != STATE_WHITESPACE) {
                size_t token_len = input_ptr - token_start;
                if (token_len > 0 && token_len < MAX_INPUT) {
                    char token[MAX_INPUT] = {0};
                    char *dest = token;
                    char *src = token_start;
                    
                    while (src < input_ptr && (size_t)(dest - token) < sizeof(token) - 1) {
                        *dest = *src;
                        dest++;
                        src++;
                    }
                    *dest = '\0';
                    
                    const char *type_names[] = {
                        "START", "IDENTIFIER", "NUMBER", 
                        "OPERATOR", "WHITESPACE", "ERROR", "END"
                    };
                    
                    if (current_state < STATE_COUNT) {
                        printf("Token %d: '%.*s' (%s)\n", 
                               ++token_count, (int)token_len, token_start, 
                               type_names[current_state]);
                    }
                }
            }
            token_start = input_ptr;
        }
        
        current_state = next_state;
        
        if (current_state != STATE_END && current_state != STATE_ERROR) {
            input_ptr++;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid expression at position %ld\n", (long)(input_ptr - input));
        return 1;
    }
    
    printf("Parsed %d tokens successfully\n", token_count);
    return 0;
}