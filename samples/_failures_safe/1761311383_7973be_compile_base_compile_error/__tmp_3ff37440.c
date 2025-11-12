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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    int start_pos = *pos;
    int start_line = *line;
    int start_column = *column;
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *column;
        return 1;
    }
    
    while (is_whitespace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
        if (input[*pos] == '\0') {
            token->type = TOKEN_EOF;
            token->value[0] = '\0';
            token->line = *line;
            token->column = *column;
            return 1;
        }
    }
    
    start_pos = *pos;
    start_line = *line;
    start_column = *column;
    
    if (isdigit(input[*pos])) {
        int i = 0;
        while (isdigit(input[*pos]) && i < 63) {
            token->value[i++] = input[(*pos)++];
            (*column)++;
        }
        if (input[*pos] == '.' && isdigit(input[*pos + 1])) {
            token->value[i++] = input[(*pos)++];
            (*column)++;
            while (isdigit(input[*pos]) && i < 63) {
                token->value[i++] = input[(*pos)++];
                (*column)++;
            }
        }
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int i = 0;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && i < 63) {
            token->value[i++] = input[(*pos)++];
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    if (input[*pos] == '"') {
        int i = 0;
        token->value[i++] = input[(*pos)++];
        (*column)++;
        while (input[*pos] != '"' && input[*pos] != '\0' && i < 63) {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                token->value[i++] = input[(*pos)++];
                (*column)++;
                token->value[i++] = input[(*pos)++];
                (*column)++;
            } else {
                token->value[i++] = input[(*pos)++];
                (*column)++;
            }
        }
        if (input[*pos] == '"') {
            token->value[i++] = input[(*pos)++];
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        int i = 0;
        while (is_operator_char(input[*pos]) && i < 63) {
            token->value[i++] = input[(*pos)++];
            (*column)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = start_line;
        token->column = start_column;
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[(*pos)++];
    token->value[1] = '\0';
    token->line = start_line;
    token->column = start_column;
    (*column)++;
    return 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch