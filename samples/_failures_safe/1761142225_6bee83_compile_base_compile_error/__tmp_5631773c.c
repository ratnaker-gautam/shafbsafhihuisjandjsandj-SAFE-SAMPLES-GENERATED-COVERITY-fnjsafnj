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
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *start, const char *end, Token *token) {
    const char *p = start;
    int has_dot = 0;
    
    if (start >= end || !token) return 0;
    
    while (p < end) {
        if (*p == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else if (!isdigit(*p)) {
            break;
        }
        p++;
    }
    
    if (p - start > 0) {
        token->type = TOKEN_NUMBER;
        size_t len = p - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        memcpy(token->value, start, len);
        token->value[len] = '\0';
        return p - start;
    }
    return 0;
}

int parse_string(const char *start, const char *end, Token *token) {
    if (start >= end || *start != '"' || !token) return 0;
    
    const char *p = start + 1;
    while (p < end && *p != '"') {
        if (*p == '\\' && p + 1 < end) p++;
        p++;
    }
    
    if (p >= end || *p != '"') return 0;
    
    size_t len = p - start - 1;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    memcpy(token->value, start + 1, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return p - start + 1;
}

int parse_identifier(const char *start, const char *end, Token *token) {
    const char *p = start;
    if (p >= end || !isalpha(*p) || !token) return 0;
    
    p++;
    while (p < end && (isalnum(*p) || *p == '_')) {
        p++;
    }
    
    size_t len = p - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return p - start;
}

int parse_operator(const char *start, const char *end, Token *token) {
    const char *p = start;
    if (p >= end || !is_operator_char(*p) || !token) return 0;
    
    p++;
    if (p < end && is_operator_char(*p)) {
        p++;
    }
    
    size_t len = p - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return p - start;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        int consumed = 0;
        Token token = {TOKEN_UNKNOWN, ""};
        
        consumed = parse_string(p, p + MAX_TOKEN_LEN, &token);
        if (!consumed) consumed = parse_number(p, p + MAX_TOKEN_LEN, &token);
        if (!consumed) consumed = parse_identifier(p, p + MAX_TOKEN_LEN, &token);
        if (!consumed) consumed = parse_operator(p, p + MAX_TOKEN_LEN, &token);
        
        if (consumed > 0) {
            tokens[token_count++] = token;
            p += consumed;
        } else {
            token.type = TOKEN_UNKNOWN;
            token.value[0] = *p;
            token.value[1] = '\0';
            tokens[token_count++] = token;
            p++;
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
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_