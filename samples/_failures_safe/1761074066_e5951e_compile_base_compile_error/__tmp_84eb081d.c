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
    
    token.type = TOKEN_INVALID;
    token.value[0] = **input;
    token.value[1] = '\0';
    (*input)++;
    return token;
}

void parse_expression(const char **input);

void parse_primary(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Null input in parse_primary\n");
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
        printf("ERROR: Unexpected token: %s\n", token.value);
    }
}

void parse_term(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Null input in parse_term\n");
        return;
    }
    
    parse_primary(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("OPERATOR: %s\n", token.value);
        parse_term(input);
    } else {
        size_t len = strlen(token.value);
        if (len > 0 && *input != NULL) {
            *input -= len;
        }
    }
}

void parse_expression(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Null input in parse_expression\n");
        return;
    }
    
    parse_term(input);
    
    struct Token token = get_next_token(input);
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("OPERATOR: %s\n", token.value);
        parse_expression(input);
    } else {
        size_t len = strlen(token.value);
        if (len > 0 && *input != NULL) {
            *input -= len;
        }
    }
}

void parse_statement(const char **input) {
    if (input == NULL || *input == NULL) {
        printf("ERROR: Null input in parse_statement\n");
        return;
    }
    
    struct Token token = get_next_token(input);
    
    if (token.type == TOKEN_IDENTIFIER) {
        printf("ASSIGNMENT: %s\n", token.value);
        
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != '=') {
            printf("ERROR: Expected '=' after identifier\n");
            return;
        }
        printf("OPERATOR: =\n");
        
        parse_expression(input);
        
        token = get_next_token(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ';') {
            printf("ERROR: Expected ';