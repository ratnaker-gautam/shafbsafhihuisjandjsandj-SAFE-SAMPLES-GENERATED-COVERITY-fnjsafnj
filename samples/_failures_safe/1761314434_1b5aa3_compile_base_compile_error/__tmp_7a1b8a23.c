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
    
    if (p == start) return 0;
    if (p - start >= MAX_TOKEN_LEN) return 0;
    
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
    
    size_t len = p - start - 1;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(output, start + 1, len);
    output[len] = '\0';
    return p - start + 1;
}

int parse_operator(const char *start, const char *end, char *output) {
    if (start >= end || !is_operator_char(*start)) return 0;
    
    const char *p = start + 1;
    if (p < end && is_operator_char(*p)) {
        if ((*start == '&' && *p == '&') || (*start == '|' && *p == '|') ||
            (*start == '=' && *p == '=') || (*start == '!' && *p == '=') ||
            (*start == '<' && *p == '=') || (*start == '>' && *p == '=')) {
            p++;
        }
    }
    
    if (p - start >= MAX_TOKEN_LEN) return 0;
    memcpy(output, start, p - start);
    output[p - start] = '\0';
    return p - start;
}

int parse_identifier(const char *start, const char *end, char *output) {
    if (start >= end || (!isalpha(*start) && *start != '_')) return 0;
    
    const char *p = start + 1;
    while (p < end && (isalnum(*p) || *p == '_')) {
        p++;
    }
    
    if (p - start >= MAX_TOKEN_LEN) return 0;
    memcpy(output, start, p - start);
    output[p - start] = '\0';
    return p - start;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        int consumed = 0;
        Token *current = tokens + token_count;
        
        if ((consumed = parse_string(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_STRING;
        } else if ((consumed = parse_number(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_NUMBER;
        } else if ((consumed = parse_operator(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_OPERATOR;
        } else if ((consumed = parse_identifier(p, p + MAX_TOKEN_LEN, current->value)) > 0) {
            current->type = TOKEN_IDENTIFIER;
        } else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *p;
            current->value[1] = '\0';
            consumed = 1;
        }
        
        p += consumed;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER