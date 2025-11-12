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
    TOKEN_INVALID
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
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
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
    
    if (*pos - start - 2 > 63) return 0;
    
    strncpy(token->value, &input[start + 1], *pos - start - 2);
    token->value[*pos - start - 2] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    
    (*pos)++;
    if (*pos < len && is_operator_char(input[*pos])) {
        if ((input[start] == '&' && input[*pos] == '&') ||
            (input[start] == '|' && input[*pos] == '|') ||
            (input[start] == '=' && input[*pos] == '=') ||
            (input[start] == '!' && input[*pos] == '=') ||
            (input[start] == '<' && input[*pos] == '=') ||
            (input[start] == '>' && input[*pos] == '=')) {
            (*pos)++;
        }
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char *input, int len, int *pos, int *line, int *column, struct Token *token) {
    while (*pos < len) {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        token->line = *line;
        token->column = *column;
        
        if (isdigit(c)) {
            if (parse_number(input, pos, len, token)) {
                *column += (int)strlen(token->value);
                return 1;
            }
        } else if (isalpha(c) || c == '_') {
            if (parse_identifier(input, pos, len, token)) {
                *column += (int)strlen(token->value);
                return 1;
            }
        } else if (c == '"') {
            if (parse_string(input, pos, len, token)) {
                *column += (int)strlen(token->value) + 2;
                return 1;
            }
        } else if (is_operator_char(c)) {
            if (parse