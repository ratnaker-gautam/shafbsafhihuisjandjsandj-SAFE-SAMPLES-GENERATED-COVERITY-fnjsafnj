//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

TokenType classify_token(const char *value) {
    if (isdigit(value[0])) {
        return TOKEN_NUMBER;
    } else if (isalpha(value[0]) || value[0] == '_') {
        return TOKEN_IDENTIFIER;
    } else if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    } else if (value[0] == '"') {
        return TOKEN_STRING;
    }
    return TOKEN_INVALID;
}

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    while (*ptr && isdigit(*ptr)) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = ptr;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!(isalpha(*ptr) || *ptr == '_')) {
        return 0;
    }
    
    ptr++;
    while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
        ptr++;
    }
    
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (!is_operator_char(*ptr)) {
        return 0;
    }
    
    ptr++;
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    char *ptr = *input;
    
    if (*ptr != '"') {
        return 0;
    }
    
    ptr++;
    while (*ptr && *ptr != '"') {
        ptr++;
    }
    
    if (*ptr != '"') {
        return 0;
    }
    
    ptr++;
    size_t len = ptr - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) {
            break;
        }
        
        Token token;
        int parsed = 0;
        
        if (isdigit(*ptr)) {
            parsed = parse_number(&ptr, &token);
        } else if (isalpha(*ptr) || *ptr == '_') {
            parsed = parse_identifier(&ptr, &token);
        } else if (is_operator_char(*ptr)) {
            parsed = parse_operator(&ptr, &token);
        } else if (*ptr == '"') {
            parsed = parse_string(&ptr, &token);
        }
        
        if (!parsed) {
            token.type = TOKEN_INVALID;
            token.value[0] = *ptr;
            token.value[1] = '\0';
            ptr++;
        }
        
        if (token_count < max_tokens) {
            tokens[token_count] = token;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_