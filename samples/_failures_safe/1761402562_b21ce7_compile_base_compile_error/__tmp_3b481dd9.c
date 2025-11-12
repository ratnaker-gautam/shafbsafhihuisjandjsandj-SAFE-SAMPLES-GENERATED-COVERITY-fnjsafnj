//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|' || c == '^' || c == '%' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';' ||
           c == '.' || c == ':' || c == '?';
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || col == NULL || token == NULL) {
        return -1;
    }
    
    int start_pos = *pos;
    int start_line = *line;
    int start_col = *col;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = *line;
        token->column = *col;
        return 0;
    }
    
    start_pos = *pos;
    start_line = *line;
    start_col = *col;
    
    if (isdigit(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (len < 63) {
                token->value[len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int len = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (len < 63) {
                token->value[len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (input[*pos] == '"') {
        int len = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                if (len < 62) {
                    token->value[len++] = input[*pos];
                    (*pos)++;
                    (*col)++;
                    token->value[len++] = input[*pos];
                } else {
                    (*pos)++;
                    (*col)++;
                }
            } else {
                if (len < 63) {
                    token->value[len++] = input[*pos];
                }
            }
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    if (is_operator_char(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && len < 63) {
            token->value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = start_line;
        token->column = start_col;
        return 0;
    }
    
    token->type = TOKEN_UNKNOWN;
    if (input[*pos] != '\0') {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        (*col)++;
    } else {
        token->value[0] = '\0';
    }
    token->line = start_line;
    token->column = start_col;
    return 0;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPER