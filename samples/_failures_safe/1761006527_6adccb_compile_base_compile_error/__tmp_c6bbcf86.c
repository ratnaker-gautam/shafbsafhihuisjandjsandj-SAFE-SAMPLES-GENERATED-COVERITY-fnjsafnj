//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (input == NULL || *input == NULL) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit((unsigned char)**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && isdigit((unsigned char)**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isalpha((unsigned char)**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum((unsigned char)**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (**input && is_operator_char(**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_INVALID;
    token.value[0] = **input;
    token.value[1] = '\0';
    (*input)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(const char **input);

void parse_primary(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("Error: Null input in parse_primary\n");
        return;
    }
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering parentheses\n");
        parse_expression(input);
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("Error: Expected closing parenthesis\n");
        } else {
            printf("Exiting parentheses\n");
        }
    } else {
        printf("Error: Unexpected token in primary expression\n");
        print_token(token);
    }
}

void parse_term(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("Error: Null input in parse_term\n");
        return;
    }
    
    parse_primary(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Found operator: %s\n", token.value);
        parse_term(input);
    } else {
        if (token.value[0] != '\0') {
            size_t len = strlen(token.value);
            if (*input - len >= *input) {
                *input -= len;
            }
        }
    }
}

void parse_expression(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("Error: Null input in parse_expression\n");
        return;
    }
    
    parse_term(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Found operator: %s\n", token.value);
        parse_expression(input);
    } else {
        if (token.value[0] != '\0') {
            size_t len = strlen(token.value);
            if (*input - len >= *input) {
                *input -= len;
            }
        }
    }
}

int main() {
    char input_buffer[1024];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (