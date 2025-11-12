//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
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

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int len = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
        len = *pos - start;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len > 0) {
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = 0;
    
    if (input[*pos] != '\0' && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        len = 1;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
            len = *pos - start;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = 0;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
        len = 1;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && len < (MAX_TOKEN_LEN - 1)) {
            (*pos)++;
            len = *pos - start;
        }
        
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = 0;
    
    if (input[*pos] == '"') {
        (*pos)++;
        len = 1;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                (*pos) += 2;
                len = *pos - start;
            } else {
                (*pos)++;
                len = *pos - start;
            }
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            len = *pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
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
        
        if (parse_number(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        if (parse_string(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        if (parse_identifier(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        if (parse_operator(input, pos, token)) {
            *column += (