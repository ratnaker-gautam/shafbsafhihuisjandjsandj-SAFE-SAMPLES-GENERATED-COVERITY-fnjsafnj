//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    
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
    
    if (isalpha((unsigned char)**input)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
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
    printf("Token: %-12s Value: %s\n", type_names[token.type], token.value);
}

void parse_expression(const char **input);

void parse_factor(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        print_token(token);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '-') {
        printf("Token: OPERATOR     Value: %s\n", token.value);
        parse_factor(input);
    } else {
        printf("Unexpected token in factor: %s\n", token.value);
    }
}

void parse_term(const char **input) {
    parse_factor(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '*' || token.value[0] == '/')) {
        print_token(token);
        parse_factor(input);
        parse_term(input);
    } else {
        *input -= strlen(token.value);
    }
}

void parse_expression(const char **input) {
    parse_term(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && 
        (token.value[0] == '+' || token.value[0] == '-')) {
        print_token(token);
        parse_term(input);
        parse_expression(input);
    } else {
        *input -= strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    const char *current = input;
    printf("\nParsing expression: %s\n", input);
    printf("Token stream:\n");
    
    parse_expression(&current);
    
    struct Token final_token = get_next_token(&current);
    if (final_token.type != TOKEN_EOF && final_token.type != TOKEN_INVALID) {
        printf("Unexpected tokens at end: ");
        while (final_token.type != TOKEN_EOF && final_token.type != TOKEN_INVALID) {
            print_token(final_token);
            final_token = get_next_token(&current);
        }
    }
    
    return 0;
}