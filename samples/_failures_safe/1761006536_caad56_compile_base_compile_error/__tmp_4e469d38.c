//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int len = (int)strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int len = (int)strlen(input);
    
    if (*pos >= len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    (*pos)++;
    
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int len = (int)strlen(input);
    
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    (*pos)++;
    
    while (*pos < len && is_operator_char(input[*pos]) && (*pos - start < 2)) {
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int len = (int)strlen(input);
    
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int col = 1;
    int len = (int)strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
            pos++;
            continue;
        }
        
        struct Token token;
        int success = 0;
        
        if (parse_number(input, &pos, line, col, &token)) success = 1;
        else if (parse_string(input, &pos, line, col, &token)) success = 1;
        else if (parse_identifier(input, &pos, line, col, &token)) success = 1;
        else if (parse_operator(input, &pos, line, col, &token)) success = 1;
        
        if (success) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
                col += (int)strlen(token.value);
            }
        } else {