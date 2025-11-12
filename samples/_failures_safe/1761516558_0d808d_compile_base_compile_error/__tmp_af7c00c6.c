//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

int parse_number(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int has_dot = 0;
    int len = strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || (input[*pos] == '.' && !has_dot))) {
        if (input[*pos] == '.') has_dot = 1;
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int len = strlen(input);
    
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    
    while (*pos < len && input[*pos] != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    if (*pos >= len || input[*pos] != '"') return 0;
    
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = column;
    (*pos)++;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int len = strlen(input);
    
    if (*pos >= len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int len = strlen(input);
    
    while (*pos < len && is_operator_char(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = column;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    int len;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) return -1;
    
    len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        if (isdigit(input[pos])) {
            if (!parse_number(input, &pos, &tokens[token_count], line, column)) return -1;
            token_count++;
            column += strlen(tokens[token_count-1].value);
            continue;
        }
        
        if (input[pos] == '"') {
            if (!parse_string(input, &pos, &tokens[token_count], line, column)) return -1;
            token_count++;
            column += strlen(tokens[token_count-1].value) + 2;
            continue;
        }
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            if (!parse_identifier(input, &pos, &tokens[token_count], line, column)) return -1;
            token_count++;
            column += strlen(tokens[token_count-1].value);
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            if (!parse_operator(input, &pos, &tokens[token_count], line, column)) return -1;
            token_count++;
            column += strlen(tokens[token_count-1].value);
            continue;
        }
        
        return -1;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TO