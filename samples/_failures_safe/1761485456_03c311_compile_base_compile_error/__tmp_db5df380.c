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

int parse_number(const char *start, const char *end, char *output) {
    const char *p = start;
    int has_dot = 0;
    
    if (p >= end) return 0;
    
    while (p < end && (isdigit(*p) || *p == '.')) {
        if (*p == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        p++;
    }
    
    if (p - start >= MAX_TOKEN_LEN) return 0;
    if (p - start == 0) return 0;
    memcpy(output, start, p - start);
    output[p - start] = '\0';
    return p - start;
}

int parse_string(const char *start, const char *end, char *output) {
    if (start >= end || *start != '"') return 0;
    
    const char *p = start + 1;
    while (p < end && *p != '"') {
        if (*p == '\\' && p + 1 < end) p++;
        p++;
    }
    
    if (p >= end || *p != '"') return 0;
    
    int len = p - start - 1;
    if (len < 0 || len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(output, start + 1, len);
    output[len] = '\0';
    return p - start + 1;
}

int parse_operator(const char *start, const char *end, char *output) {
    const char *p = start;
    if (p >= end) return 0;
    
    while (p < end && is_operator_char(*p) && (p - start) < 2) {
        p++;
    }
    
    int len = p - start;
    if (len == 0 || len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(output, start, len);
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *start, const char *end, char *output) {
    const char *p = start;
    if (p >= end || !isalpha(*p)) return 0;
    
    p++;
    while (p < end && (isalnum(*p) || *p == '_')) {
        p++;
    }
    
    int len = p - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(output, start, len);
    output[len] = '\0';
    return len;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        char buffer[MAX_TOKEN_LEN];
        int consumed = 0;
        
        if ((consumed = parse_string(p, p + MAX_TOKEN_LEN, buffer)) > 0) {
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            p += consumed;
            token_count++;
            continue;
        }
        
        if ((consumed = parse_number(p, p + MAX_TOKEN_LEN, buffer)) > 0) {
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            p += consumed;
            token_count++;
            continue;
        }
        
        if ((consumed = parse_operator(p, p + MAX_TOKEN_LEN, buffer)) > 0) {
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            p += consumed;
            token_count++;
            continue;
        }
        
        if ((consumed = parse_identifier(p, p + MAX_TOKEN_LEN, buffer)) > 0) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            p += consumed;
            token