//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        size_t len = (size_t)(*pos - start);
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
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            (*pos)++;
        }
        
        size_t len = (size_t)(*pos - start);
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
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        while (is_operator_char(input[*pos]) && (*pos - start) < (MAX_TOKEN_LEN - 1)) {
            (*pos)++;
        }
        
        size_t len = (size_t)(*pos - start);
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
    
    if (input[*pos] == '"') {
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            size_t len = (size_t)(*pos - start);
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

void skip_whitespace(const char *input, int *pos, int *line, int *column) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    skip_whitespace(input, pos, line, column);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 1;
    }
    
    token->line = *line;
    token->column = *column;
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*column)++;
    return 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENT