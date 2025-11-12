//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int has_decimal = 0;
    
    while (isdigit(input[*pos]) || (input[*pos] == '.' && !has_decimal)) {
        if (input[*pos] == '.') has_decimal = 1;
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
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
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
    
    if (!is_operator_char(input[*pos])) return 0;
    
    while (is_operator_char(input[*pos]) && i < 2) {
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
    
    if (input[*pos] != '"') return 0;
    
    token->value[i++] = input[*pos];
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
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
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        struct Token token;
        int success = 0;
        
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, &token, line, column);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            success = parse_identifier(input, &pos, &token, line, column);
        } else if (is_operator_char(input[pos])) {
            success = parse_operator(input, &pos, &token, line, column);
        } else if (input[pos] == '"') {
            success = parse_string(input, &pos, &token, line, column);
        }
        
        if (success) {
            tokens[token_count++] = token;
            column += strlen(token.value);
        } else {
            pos++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: