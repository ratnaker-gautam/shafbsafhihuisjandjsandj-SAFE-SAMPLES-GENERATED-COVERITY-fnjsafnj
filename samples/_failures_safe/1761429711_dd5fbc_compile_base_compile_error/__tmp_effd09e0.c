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
    char value[32];
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
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    while (*pos < len) {
        char c = input[*pos];
        if (isalnum(c) || c == '_') {
            (*pos)++;
        } else {
            break;
        }
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
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
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        char first = input[start];
        char second = input[*pos];
        if ((first == '&' && second == '&') || (first == '|' && second == '|') ||
            (first == '=' && second == '=') || (first == '!' && second == '=') ||
            (first == '<' && second == '=') || (first == '>' && second == '=')) {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
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
                *column += (int)strlen(token->value);
                return 1;
            }
        } else if (is_operator_char(c)) {
            if (parse_operator(input, pos, len, token)) {
                *column += (int)strlen(token->value);
                return 1;
            }
        } else {
            token->type = TOKEN_ERROR;
            token->value[0] = c;
            token