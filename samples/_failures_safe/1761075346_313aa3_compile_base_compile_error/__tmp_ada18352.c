//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char **input) {
    const char *start = *input;
    char op[3] = {0};
    
    if (**input == '\0') return 0;
    
    op[0] = **input;
    (*input)++;
    
    if (**input != '\0') {
        if ((*start == '&' && **input == '&') ||
            (*start == '|' && **input == '|') ||
            (*start == '=' && **input == '=') ||
            (*start == '!' && **input == '=') ||
            (*start == '<' && **input == '=') ||
            (*start == '>' && **input == '=')) {
            op[1] = **input;
            (*input)++;
        }
    }
    
    return (int)(*input - start);
}

int parse_number(const char **input, char *buffer, size_t buf_size) {
    const char *start = *input;
    size_t len = 0;
    
    if (buf_size == 0) return 0;
    
    while (**input != '\0' && isdigit(**input)) {
        if (len < buf_size - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_string(const char **input, char *buffer, size_t buf_size) {
    const char *start = *input;
    size_t len = 0;
    
    if (buf_size == 0) return 0;
    if (**input != '"') return 0;
    (*input)++;
    
    while (**input != '\0' && **input != '"') {
        if (len < buf_size - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
    }
    
    if (**input == '"') {
        (*input)++;
        buffer[len] = '\0';
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char **input, char *buffer, size_t buf_size) {
    const char *start = *input;
    size_t len = 0;
    
    if (buf_size == 0) return 0;
    
    if (**input != '\0' && (isalpha(**input) || **input == '_')) {
        if (len < buf_size - 1) {
            buffer[len++] = **input;
        }
        (*input)++;
        
        while (**input != '\0' && (isalnum(**input) || **input == '_')) {
            if (len < buf_size - 1) {
                buffer[len++] = **input;
            }
            (*input)++;
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    *input = start;
    return 0;
}

void skip_whitespace(const char **input) {
    while (**input != '\0' && isspace(**input)) {
        (*input)++;
    }
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    size_t token_count = 0;
    const char *current = input;
    
    if (input == NULL || tokens == NULL || max_tokens == 0) {
        return 0;
    }
    
    while (*current != '\0' && token_count < max_tokens) {
        skip_whitespace(&current);
        if (*current == '\0') break;
        
        Token *token = tokens + token_count;
        
        if (parse_string(&current, token->value, sizeof(token->value))) {
            token->type = TOKEN_STRING;
            token_count++;
        } else if (parse_number(&current, token->value, sizeof(token->value))) {
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_identifier(&current, token->value, sizeof(token->value))) {
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (is_operator_char(*current)) {
            int op_len = parse_operator(&current);
            if (op_len > 0 && op_len < (int)sizeof(token->value)) {
                strncpy(token->value, current - op_len, op_len);
                token->value[op_len] = '\0';
                token->type = TOKEN_OPERATOR;
                token_count++;
            } else {
                token->type = TOKEN_INVALID;
                token_count++;
                break;
            }
        } else {
            token->type = TOKEN_INVALID;
            token_count++;
            break;
        }
    }