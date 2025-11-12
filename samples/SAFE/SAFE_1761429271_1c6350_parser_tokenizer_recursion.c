//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_ERROR, ""};
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace((unsigned char)**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit((unsigned char)**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator(**input)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
        return token;
    }
    
    token.type = TOKEN_ERROR;
    return token;
}

int parse_expression(const char **input, int *result);

int parse_term(const char **input, int *result) {
    int left_result;
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        *result = atoi(token.value);
        return 1;
    }
    
    if (token.type == TOKEN_OPERATOR && token.value[0] == '+') {
        if (!parse_term(input, &left_result)) {
            return 0;
        }
        *result = left_result;
        return 1;
    }
    
    if (token.type == TOKEN_OPERATOR && token.value[0] == '-') {
        if (!parse_term(input, &left_result)) {
            return 0;
        }
        *result = -left_result;
        return 1;
    }
    
    return 0;
}

int parse_expression(const char **input, int *result) {
    int left_result, right_result;
    
    if (!parse_term(input, &left_result)) {
        return 0;
    }
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_EOF) {
        *result = left_result;
        return 1;
    }
    
    if (token.type != TOKEN_OPERATOR) {
        return 0;
    }
    
    if (!parse_expression(input, &right_result)) {
        return 0;
    }
    
    switch (token.value[0]) {
        case '+':
            *result = left_result + right_result;
            break;
        case '-':
            *result = left_result - right_result;
            break;
        case '*':
            *result = left_result * right_result;
            break;
        case '/':
            if (right_result == 0) {
                return 0;
            }
            *result = left_result / right_result;
            break;
        default:
            return 0;
    }
    
    return 1;
}

int main(void) {
    char input[256];
    printf("Enter arithmetic expression: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    const char *input_ptr = input;
    int result;
    
    if (parse_expression(&input_ptr, &result)) {
        struct Token final_token = get_next_token(&input_ptr);
        if (final_token.type == TOKEN_EOF) {
            printf("Result: %d\n", result);
        } else {
            printf("Invalid expression: unexpected token\n");
        }
    } else {
        printf("Invalid expression: parsing failed\n");
    }
    
    return 0;
}