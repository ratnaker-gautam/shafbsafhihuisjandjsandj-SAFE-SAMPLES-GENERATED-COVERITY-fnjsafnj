//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

int parse_string_literal(char **input, char *output) {
    char *start = *input;
    if (*start != '"') return 0;
    
    start++;
    char *out_ptr = output;
    
    while (*start && *start != '"') {
        if (out_ptr - output >= MAX_TOKEN_LEN - 1) return 0;
        *out_ptr++ = *start++;
    }
    
    if (*start != '"') return 0;
    
    *out_ptr = '\0';
    *input = start + 1;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    Token *current = tokens;
    char *ptr = input;
    
    while (*ptr && (current - tokens) < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        char token_value[MAX_TOKEN_LEN] = {0};
        char *val_ptr = token_value;
        
        if (*ptr == '"') {
            if (!parse_string_literal(&ptr, token_value)) {
                current->type = TOKEN_INVALID;
                strncpy(current->value, "INVALID_STRING", MAX_TOKEN_LEN - 1);
                current->value[MAX_TOKEN_LEN - 1] = '\0';
                current++;
                continue;
            }
            current->type = TOKEN_STRING;
            strncpy(current->value, token_value, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            current++;
            continue;
        }
        
        if (is_operator_char(*ptr)) {
            *val_ptr++ = *ptr++;
            if (is_operator_char(*ptr)) {
                *val_ptr++ = *ptr++;
            }
            *val_ptr = '\0';
            current->type = TOKEN_OPERATOR;
            strncpy(current->value, token_value, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            current++;
            continue;
        }
        
        if (isdigit(*ptr)) {
            while (*ptr && (isdigit(*ptr) || *ptr == '.')) {
                if (val_ptr - token_value >= MAX_TOKEN_LEN - 1) break;
                *val_ptr++ = *ptr++;
            }
            *val_ptr = '\0';
            current->type = TOKEN_NUMBER;
            strncpy(current->value, token_value, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            current++;
            continue;
        }
        
        if (isalpha(*ptr) || *ptr == '_') {
            while (*ptr && (isalnum(*ptr) || *ptr == '_')) {
                if (val_ptr - token_value >= MAX_TOKEN_LEN - 1) break;
                *val_ptr++ = *ptr++;
            }
            *val_ptr = '\0';
            current->type = TOKEN_IDENTIFIER;
            strncpy(current->value, token_value, MAX_TOKEN_LEN - 1);
            current->value[MAX_TOKEN_LEN - 1] = '\0';
            current++;
            continue;
        }
        
        current->type = TOKEN_INVALID;
        *val_ptr++ = *ptr++;
        *val_ptr = '\0';
        strncpy(current->value, token_value, MAX_TOKEN_LEN - 1);
        current->value[MAX_TOKEN_LEN - 1] = '\0';
        current++;
    }
    
    if (current - tokens < max_tokens) {
        current->type = TOKEN_EOF;
        strncpy(current->value, "EOF", MAX_TOKEN_LEN - 1);
        current->value[MAX_TOKEN_LEN - 1] = '\0';
        current++;
    }
    
    return current - tokens;
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN