//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    if (!input || !pos || !token || *pos < 0 || len < 0 || *pos >= len) return 0;
    
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
    
    int length = *pos - start;
    if (length <= 0) return 0;
    if (length > 31) length = 31;
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    if (!input || !pos || !token || *pos < 0 || len < 0 || *pos >= len) return 0;
    
    int start = *pos;
    
    if (!isalpha(input[start]) && input[start] != '_') return 0;
    (*pos)++;
    
    while (*pos < len) {
        char c = input[*pos];
        if (isalnum(c) || c == '_') {
            (*pos)++;
        } else {
            break;
        }
    }
    
    int length = *pos - start;
    if (length <= 0) return 0;
    if (length > 31) length = 31;
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (!input || !pos || !token || *pos < 0 || len < 0 || *pos >= len) return 0;
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < len) {
        char c = input[*pos];
        if (c == '"') {
            (*pos)++;
            break;
        } else if (c == '\\') {
            (*pos)++;
            if (*pos >= len) break;
        }
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length <= 0) return 0;
    if (length > 31) length = 31;
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (!input || !pos || !token || *pos < 0 || len < 0 || *pos >= len) return 0;
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        char op1 = input[start];
        char op2 = input[*pos];
        if ((op1 == '&' && op2 == '&') || (op1 == '|' && op2 == '|') ||
            (op1 == '=' && op2 == '=') || (op1 == '!' && op2 == '=') ||
            (op1 == '<' && op2 == '=') || (op1 == '>' && op2 == '=')) {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length <= 0) return 0;
    if (length > 31) length = 31;
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char *input, int *pos, int len, struct Token *token, int *line, int *col) {
    if (!input || !pos || !token || !line || !col || *pos < 0 || len < 0) return 0;
    
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *col;
        return 1;