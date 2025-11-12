//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int input_len = strlen(input);
    
    while (*pos < input_len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int input_len = strlen(input);
    
    if (*pos < input_len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int input_len = strlen(input);
    
    if (*pos < input_len && is_operator_char(input[*pos])) {
        (*pos)++;
        if (*pos < input_len && is_operator_char(input[*pos])) {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int input_len = strlen(input);
    
    if (*pos < input_len && input[*pos] == '"') {
        (*pos)++;
        while (*pos < input_len && input[*pos] != '"' && input[*pos] != '\0') {
            (*pos)++;
        }
        
        if (*pos < input_len && input[*pos] == '"') {
            (*pos)++;
            int len = *pos - start;
            if (len >= MAX_TOKEN_LEN) return 0;
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int line, int *column, struct Token *token) {
    int input_len = strlen(input);
    
    while (*pos < input_len && input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                line++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        token->line = line;
        token->column = *column;
        
        if (parse_number(input, pos, token)) {
            *column += strlen(token->value);
            return 1;
        }
        if (parse_identifier(input, pos, token)) {
            *column += strlen(token->value);
            return 1;
        }
        if (parse_operator(input, pos, token)) {
            *column += strlen(token->value);
            return 1;
        }
        if (parse_string(input, pos, token)) {
            *column += strlen(token->value);
            return 1;
        }
        
        token->type = TOKEN_UNKNOWN;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        (*column)++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 1;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN