//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int has_dot = 0;
    int len = (int)strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
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

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int len = (int)strlen(input);
    
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
    int len = (int)strlen(input);
    
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    
    while (*pos < len && is_operator_char(input[*pos]) && i < 2) {
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

int parse_string(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int len = (int)strlen(input);
    
    if (*pos >= len || input[*pos] != '"') return 0;
    
    token->value[i++] = input[*pos];
    (*pos)++;
    
    while (*pos < len && input[*pos] != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    if (*pos < len && input[*pos] == '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = column;
        return 1;
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    int len = (int)strlen(input);
    
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
        int success = 0;
        
        if (parse_number(input, &pos, &token, line, column)) success = 1;
        else if (parse_identifier(input, &pos, &token, line, column)) success = 1;
        else if (parse_operator(input, &pos, &token, line, column)) success = 1;
        else if (parse_string(input, &pos, &token, line, column)) success = 1;
        
        if (success) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
                column += (int)strlen(token.value);
            }
        } else {
            pos++;
            column++;
        }