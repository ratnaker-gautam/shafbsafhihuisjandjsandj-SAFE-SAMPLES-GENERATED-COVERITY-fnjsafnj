//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int len = strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos >= len || input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < len && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        int value_len = *pos - start - 2;
        if (value_len >= 0 && value_len < MAX_TOKEN_LEN) {
            token->type = TOKEN_STRING;
            strncpy(token->value, &input[start + 1], value_len);
            token->value[value_len] = '\0';
            return 1;
        }
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    int start = *pos;
    
    if (*pos >= len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    (*pos)++;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    int start = *pos;
    
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    
    (*pos)++;
    while (*pos < len && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (pos >= len) break;
        
        struct Token token;
        token.line = line;
        token.column = column;
        
        int parsed = 0;
        parsed = parse_number(input, &pos, &token) ||
                parse_string(input, &pos, &token) ||
                parse_identifier(input, &pos, &token) ||
                parse_operator(input, &pos, &token);
        
        if (!parsed) {
            token.type = TOKEN_UNKNOWN;
            if (pos < len) {
                token.value[0] = input[pos];
                token.value[1] = '\0';
                pos++;
            } else {
                token.value[0] = '\0';
            }
        }
        
        if (token_count < max_tokens) {
            tokens[token_count] = token;
            token_count++;
        }
        
        int token_len = strlen(token.value);
        if (token_len > 0) {
            column += token_len;
        } else {
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].