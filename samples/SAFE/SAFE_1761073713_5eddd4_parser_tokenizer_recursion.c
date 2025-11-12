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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
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
    
    if (isalpha((unsigned char)**input) || **input == '_') {
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
    printf("Token: %-12s Value: '%s'\n", type_names[token.type], token.value);
}

void parse_expression(const char **input);

void parse_factor(const char **input) {
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        print_token(token);
    } else if (token.type == TOKEN_IDENTIFIER) {
        print_token(token);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Token: LPAREN       Value: '('\n");
        parse_expression(input);
        token = get_next_token(input);
        if (token.type == TOKEN_OPERATOR && token.value[0] == ')') {
            printf("Token: RPAREN       Value: ')'\n");
        } else {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Unexpected token in factor\n");
        print_token(token);
    }
}

void parse_term(const char **input) {
    parse_factor(input);
    
    struct Token token = get_next_token(input);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '*' || token.value[0] == '/')) {
        print_token(token);
        parse_factor(input);
        token = get_next_token(input);
    }
    
    (*input)--;
}

void parse_expression(const char **input) {
    parse_term(input);
    
    struct Token token = get_next_token(input);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '+' || token.value[0] == '-')) {
        print_token(token);
        parse_term(input);
        token = get_next_token(input);
    }
    
    (*input)--;
}

int main() {
    char input[1024];
    
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
    
    printf("\nToken stream:\n");
    const char *current = input;
    
    struct Token token;
    do {
        token = get_next_token(&current);
        if (token.type != TOKEN_EOF) {
            print_token(token);
        }
    } while (token.type != TOKEN_EOF);
    
    printf("\nParse tree:\n");
    current = input;
    parse_expression(&current);
    
    token = get_next_token(&current);
    if (token.type != TOKEN_EOF) {
        printf("Error: Unexpected tokens at end of input\n");
        print_token(token);
    }
    
    return 0;
}