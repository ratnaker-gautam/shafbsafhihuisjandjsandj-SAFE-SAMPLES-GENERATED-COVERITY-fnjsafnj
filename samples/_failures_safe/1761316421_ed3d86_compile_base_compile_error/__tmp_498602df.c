//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (*pos < len) {
        char c = input[*pos];
        if (isdigit(c)) {
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (*pos - start > 0) {
        int copy_len = *pos - start;
        if (copy_len >= 64) copy_len = 63;
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        int copy_len = *pos - start;
        if (copy_len >= 64) copy_len = 63;
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (*pos < len && input[*pos] == '"') {
        int start = *pos;
        (*pos)++;
        
        while (*pos < len && input[*pos] != '"') {
            if (input[*pos] == '\\' && *pos + 1 < len) {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (*pos < len && input[*pos] == '"') {
            (*pos)++;
            int copy_len = *pos - start;
            if (copy_len >= 64) copy_len = 63;
            strncpy(token->value, &input[start], copy_len);
            token->value[copy_len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
        *pos = start;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (*pos < len && is_operator_char(input[*pos])) {
        int start = *pos;
        (*pos)++;
        
        if (*pos < len && is_operator_char(input[*pos])) {
            char op1 = input[start];
            char op2 = input[*pos];
            if ((op1 == '=' && op2 == '=') || (op1 == '!' && op2 == '=') ||
                (op1 == '<' && op2 == '=') || (op1 == '>' && op2 == '=') ||
                (op1 == '&' && op2 == '&') || (op1 == '|' && op2 == '|')) {
                (*pos)++;
            }
        }
        
        int copy_len = *pos - start;
        if (copy_len >= 64) copy_len = 63;
        strncpy(token->value, &input[start], copy_len);
        token->value[copy_len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

void skip_whitespace(const char *input, int *pos, int len, int *line, int *col) {
    while (*pos < len) {
        char c = input[*pos];
        if (c == ' ' || c == '\t') {
            (*pos)++;
            (*col)++;
        } else if (c == '\n') {
            (*pos)++;
            (*line)++;
            *col = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(const char *input, int *pos, int len, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    
    skip_whitespace(input, pos, len, line, col);
    token.line = *line;
    token.column = *col;
    
    if (*pos >= len) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char c = input[*