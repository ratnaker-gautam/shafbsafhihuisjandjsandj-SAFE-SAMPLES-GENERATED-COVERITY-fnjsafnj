//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, Token *token) {
    int start = *pos;
    int has_dot = 0;
    int input_len = strlen(input);
    
    if (start >= input_len) return 0;
    
    while (*pos < input_len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, Token *token) {
    int input_len = strlen(input);
    if (*pos >= input_len || input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < input_len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < input_len) (*pos)++;
        (*pos)++;
    }
    
    if (*pos < input_len && input[*pos] == '"') {
        int copy_len = *pos - start + 1;
        if (copy_len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], copy_len);
            token->value[copy_len] = '\0';
            token->type = TOKEN_STRING;
            (*pos)++;
            return 1;
        }
    }
    return 0;
}

int parse_operator(const char *input, int *pos, Token *token) {
    int input_len = strlen(input);
    if (*pos >= input_len || !is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < input_len && is_operator_char(input[*pos]) && *pos - start < MAX_TOKEN_LEN - 1) {
        (*pos)++;
    }
    
    int copy_len = *pos - start;
    if (copy_len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, Token *token) {
    int input_len = strlen(input);
    if (*pos >= input_len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int copy_len = *pos - start;
    if (copy_len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int input_len = strlen(input);
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    while (pos < input_len && token_count < max_tokens) {
        while (pos < input_len && isspace(input[pos])) pos++;
        if (pos >= input_len) break;
        
        Token token;
        memset(&token, 0, sizeof(Token));
        
        if (parse_string(input, &pos, &token) ||
            parse_number(input, &pos, &token) ||
            parse_operator(input, &pos, &token) ||
            parse_identifier(input, &pos, &token)) {
            
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
            }
        } else {
            if (token_count < max_tokens) {
                token.value[0]