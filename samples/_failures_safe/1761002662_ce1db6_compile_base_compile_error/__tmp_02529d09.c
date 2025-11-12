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
    TOKEN_EOF,
    TOKEN_INVALID
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void skip_whitespace(const char *input, int *pos, int *line, int *col) {
    while (input[*pos] != '\0') {
        if (input[*pos] == ' ') {
            (*pos)++;
            (*col)++;
        } else if (input[*pos] == '\t') {
            (*pos)++;
            (*col) += 4;
        } else if (input[*pos] == '\n') {
            (*pos)++;
            (*line)++;
            *col = 1;
        } else {
            break;
        }
    }
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int col) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int col) {
    int start = *pos;
    
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int col) {
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        if (is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int col) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int len = *pos - start;
        if (len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token->line = line;
            token->column = col;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int col = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos, &line, &col);
        
        if (input[pos] == '\0') break;
        
        struct Token token;
        int success = 0;
        
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, &token, line, col);
        } else if (is_valid_identifier_start(input[pos])) {
            success = parse_identifier(input, &pos, &token, line,