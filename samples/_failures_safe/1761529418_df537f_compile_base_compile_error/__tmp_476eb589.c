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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
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
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!is_valid_identifier_start(input[*pos])) return 0;
    (*pos)++;
    
    while (is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*pos)++;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int skip_whitespace(const char *input, int *pos) {
    int skipped = 0;
    while (isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            return 2;
        }
        (*pos)++;
        skipped = 1;
    }
    return skipped;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    token->line = *line;
    token->column = *column;
    
    int whitespace_result = skip_whitespace(input, pos);
    if (whitespace_result == 2) {
        (*line)++;
        *column = 1;
    } else if (whitespace_result) {
        *column = 1;
    }
    
    if (parse_number(input, pos, token)) {
        *column += strlen(token->value);
        return 1;
    }
    
    if (parse_identifier(input, pos, token)) {
        *column += strlen(token->value);
        return 1;
    }
    
    if (parse_operator(input, pos, token)) {
        (*column)++;
        return 1;
    }
    
    if (parse_string(input, pos, token)) {
        *column += strlen(token->value);
        return 1;
    }
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    (*column)++;
    return 1;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf