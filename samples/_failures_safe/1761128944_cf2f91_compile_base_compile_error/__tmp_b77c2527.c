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
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, struct Token *token) {
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
    if (len > 31) len = 31;
    if (len <= 0) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int start = *pos;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    
    int len = *pos - start;
    if (len > 31) len = 31;
    if (len < 0) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 31) len = 31;
    if (len <= 0) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        if ((input[start] == '&' && input[*pos] == '&') ||
            (input[start] == '|' && input[*pos] == '|') ||
            (input[start] == '=' && input[*pos] == '=') ||
            (input[start] == '!' && input[*pos] == '=') ||
            (input[start] == '<' && input[*pos] == '=') ||
            (input[start] == '>' && input[*pos] == '=')) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 31) len = 31;
    if (len <= 0) return 0;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void skip_whitespace(const char *input, int *pos, int *line, int *column) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
        } else {
            break;
        }
    }
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    skip_whitespace(input, pos, line, column);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    token->line = *line;
    token->column = *column;
    
    if (parse_number(input, pos, token)) {
        *column += (int)strlen(token->value);
        return 1;
    }
    
    if (parse_string(input, pos, token)) {
        *column += (int)strlen(token->value) + 2;
        return 1;
    }