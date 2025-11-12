//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUM,
    STATE_READING_OP,
    STATE_ERROR,
    STATE_DONE
} calculator_state_t;

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_END,
    TOKEN_INVALID
} token_type_t;

typedef struct {
    token_type_t type;
    int value;
    char op;
} token_t;

static int parse_number(const char *str, size_t len, int *result) {
    if (len == 0 || len > 10) return 0;
    
    int value = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(str[i])) return 0;
        if (value > (INT_MAX - (str[i] - '0')) / 10) return 0;
        value = value * 10 + (str[i] - '0');
    }
    
    *result = value;
    return 1;
}

static token_t get_next_token(const char **input) {
    token_t token = {TOKEN_INVALID, 0, 0};
    
    while (**input && isspace(**input)) (*input)++;
    
    if (!**input) {
        token.type = TOKEN_END;
        return token;
    }
    
    if (isdigit(**input)) {
        const char *start = *input;
        while (isdigit(**input)) (*input)++;
        size_t len = *input - start;
        
        if (parse_number(start, len, &token.value)) {
            token.type = TOKEN_NUMBER;
        } else {
            token.type = TOKEN_INVALID;
        }
        return token;
    }
    
    if (**input == '+' || **input == '-' || **input == '*' || **input == '/') {
        token.type = TOKEN_OPERATOR;
        token.op = **input;
        (*input)++;
        return token;
    }
    
    token.type = TOKEN_INVALID;
    return token;
}

static int perform_operation(int left, char op, int right, int *result) {
    switch (op) {
        case '+':
            if ((right > 0 && left > INT_MAX - right) || 
                (right < 0 && left < INT_MIN - right)) return 0;
            *result = left + right;
            break;
        case '-':
            if ((right < 0 && left > INT_MAX + right) || 
                (right > 0 && left < INT_MIN + right)) return 0;
            *result = left - right;
            break;
        case '*':
            if (left != 0 && right != 0) {
                if (left > INT_MAX / right || left < INT_MIN / right) return 0;
                if ((left == -1 && right == INT_MIN) || (right == -1 && left == INT_MIN)) return 0;
            }
            *result = left * right;
            break;
        case '/':
            if (right == 0) return 0;
            if (left == INT_MIN && right == -1) return 0;
            *result = left / right;
            break;
        default:
            return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter simple arithmetic expression (e.g., 5 + 3 * 2): ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    const char *cursor = input;
    calculator_state_t state = STATE_START;
    int result = 0;
    int current_value = 0;
    char current_op = '+';
    token_t token;
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        switch (state) {
            case STATE_START:
                token = get_next_token(&cursor);
                if (token.type == TOKEN_NUMBER) {
                    result = token.value;
                    state = STATE_READING_OP;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_OP:
                token = get_next_token(&cursor);
                if (token.type == TOKEN_OPERATOR) {
                    current_op = token.op;
                    state = STATE_READING_NUM;
                } else if (token.type == TOKEN_END) {
                    state = STATE_DONE;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUM:
                token = get_next_token(&cursor);
                if (token.type == TOKEN_NUMBER) {
                    current_value = token.value;
                    if (!perform_operation(result, current_op, current_value, &result)) {
                        state = STATE_ERROR;
                    } else {
                        state = STATE_READING_OP;
                    }
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid expression\n");
        return 1;
    }
    
    printf("Result: %