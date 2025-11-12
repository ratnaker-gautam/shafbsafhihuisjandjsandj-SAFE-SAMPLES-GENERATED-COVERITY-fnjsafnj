//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    char op[3] = {0};
    
    if (!input || !*input || !token) return 0;
    
    op[0] = **input;
    (*input)++;
    
    if (**input && is_operator_char(**input)) {
        if ((*start == '<' && **input == '=') ||
            (*start == '>' && **input == '=') ||
            (*start == '!' && **input == '=') ||
            (*start == '=' && **input == '=') ||
            (*start == '&' && **input == '&') ||
            (*start == '|' && **input == '|')) {
            op[1] = **input;
            (*input)++;
        }
    }
    
    if (strlen(op) > sizeof(token->value) - 1) {
        return 0;
    }
    strncpy(token->value, op, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char num[MAX_TOKEN_LEN] = {0};
    int pos = 0;
    
    if (!input || !*input || !token) return 0;
    
    while (**input && isdigit(**input)) {
        if (pos < (int)sizeof(num) - 1) {
            num[pos++] = **input;
        }
        (*input)++;
    }
    
    if (pos == 0) {
        return 0;
    }
    
    if (strlen(num) > sizeof(token->value) - 1) {
        return 0;
    }
    strncpy(token->value, num, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char ident[MAX_TOKEN_LEN] = {0};
    int pos = 0;
    
    if (!input || !*input || !token) return 0;
    
    if (!isalpha(**input) && **input != '_') {
        return 0;
    }
    
    while (**input && (isalnum(**input) || **input == '_')) {
        if (pos < (int)sizeof(ident) - 1) {
            ident[pos++] = **input;
        }
        (*input)++;
    }
    
    if (strlen(ident) > sizeof(token->value) - 1) {
        return 0;
    }
    strncpy(token->value, ident, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *start = *input;
    char str[MAX_TOKEN_LEN] = {0};
    int pos = 0;
    
    if (!input || !*input || !token) return 0;
    
    if (**input != '"') {
        return 0;
    }
    (*input)++;
    
    while (**input && **input != '"') {
        if (pos < (int)sizeof(str) - 1) {
            str[pos++] = **input;
        }
        (*input)++;
        
        if (!**input) {
            return 0;
        }
    }
    
    if (**input != '"') {
        return 0;
    }
    (*input)++;
    
    if (strlen(str) > sizeof(token->value) - 1) {
        return 0;
    }
    strncpy(token->value, str, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    if (!input || !tokens || max_tokens <= 0) {
        return -1;
    }
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) {
            break;
        }
        
        Token *current = tokens + token_count;
        
        if (