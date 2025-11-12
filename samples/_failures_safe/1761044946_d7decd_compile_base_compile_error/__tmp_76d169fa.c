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

int parse_operator(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p == '\0') return 0;
    
    *out = *p;
    out++;
    p++;
    
    if ((*input == '<' && *p == '=') ||
        (*input == '>' && *p == '=') ||
        (*input == '!' && *p == '=') ||
        (*input == '=' && *p == '=') ||
        (*input == '&' && *p == '&') ||
        (*input == '|' && *p == '|')) {
        *out = *p;
        out++;
        p++;
    }
    
    *out = '\0';
    return p - input;
}

int parse_string(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p != '"') return 0;
    p++;
    
    while (*p != '\0' && *p != '"') {
        if (out - output >= MAX_TOKEN_LEN - 1) return -1;
        *out = *p;
        out++;
        p++;
    }
    
    if (*p != '"') return -1;
    p++;
    *out = '\0';
    return p - input;
}

int parse_number(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isdigit(*p)) return 0;
    
    while (*p != '\0' && isdigit(*p)) {
        if (out - output >= MAX_TOKEN_LEN - 1) return -1;
        *out = *p;
        out++;
        p++;
    }
    
    *out = '\0';
    return p - input;
}

int parse_identifier(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    while (*p != '\0' && (isalnum(*p) || *p == '_')) {
        if (out - output >= MAX_TOKEN_LEN - 1) return -1;
        *out = *p;
        out++;
        p++;
    }
    
    *out = '\0';
    return p - input;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *token = tokens;
    int token_count = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        int advance;
        char buffer[MAX_TOKEN_LEN];
        
        if (*p == '"') {
            advance = parse_string(p, buffer);
            if (advance > 0) {
                token->type = TOKEN_STRING;
                strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
                token->value[MAX_TOKEN_LEN - 1] = '\0';
                token++;
                token_count++;
                p += advance;
                continue;
            }
        }
        
        if (isdigit(*p)) {
            advance = parse_number(p, buffer);
            if (advance > 0) {
                token->type = TOKEN_NUMBER;
                strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
                token->value[MAX_TOKEN_LEN - 1] = '\0';
                token++;
                token_count++;
                p += advance;
                continue;
            }
        }
        
        if (isalpha(*p) || *p == '_') {
            advance = parse_identifier(p, buffer);
            if (advance > 0) {
                token->type = TOKEN_IDENTIFIER;
                strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
                token->value[MAX_TOKEN_LEN - 1] = '\0';
                token++;
                token_count++;
                p += advance;
                continue;
            }
        }
        
        if (is_operator_char(*p)) {
            advance = parse_operator(p, buffer);
            if (advance > 0) {
                token->type = TOKEN_OPERATOR;
                strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
                token->value[MAX_TOKEN_LEN - 1] = '\0';
                token++;
                token_count++;
                p += advance;
                continue;
            }
        }
        
        token->type = TOKEN_INVALID;
        token