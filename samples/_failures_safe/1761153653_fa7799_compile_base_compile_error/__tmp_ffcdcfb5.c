//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == ';';
}

struct Token get_next_token(const char **input) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (input == NULL || *input == NULL) {
        token.type = TOKEN_EOF;
        return token;
    }
    
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
    
    (*input)++;
    return token;
}

void parse_expression(const char **input);

void parse_factor(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Invalid input pointer\n");
        return;
    }
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_NUMBER) {
        printf("NUMBER: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("IDENTIFIER: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        parse_expression(input);
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            printf("ERROR: Expected ')'\n");
        }
    } else {
        printf("ERROR: Unexpected token\n");
    }
}

void parse_term(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Invalid input pointer\n");
        return;
    }
    
    parse_factor(input);
    
    struct Token token = get_next_token(input);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_factor(input);
        token = get_next_token(input);
    }
    
    if (token.type != TOKEN_EOF) {
        const char *current = *input;
        if (current > token.value) {
            (*input)--;
        }
    }
}

void parse_expression(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Invalid input pointer\n");
        return;
    }
    
    parse_term(input);
    
    struct Token token = get_next_token(input);
    while (token.type == TOKEN_OPERATOR && 
           (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(input);
        token = get_next_token(input);
    }
    
    if (token.type != TOKEN_EOF) {
        const char *current = *input;
        if (current > token.value) {
            (*input)--;
        }
    }
}

void parse_statement(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Invalid input pointer\n");
        return;
    }
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("ASSIGNMENT: %s\n", token.value);
        
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != '=') {
            printf("ERROR: Expected '='\n");
            return;
        }
        printf("OPERATOR: =\n");
        
        parse_expression(input);
        
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ';') {
            printf("ERROR: Expected ';'\n");
        }
    } else {
        printf("ERROR: Expected identifier\n");
    }