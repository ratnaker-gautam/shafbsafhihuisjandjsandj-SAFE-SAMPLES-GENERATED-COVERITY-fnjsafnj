//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 31) return 0;
    if (*pos - start == 0) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') (*pos)++;
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int length = *pos - start;
    if (length > 31) return 0;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 31) return 0;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 31) return 0;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || col == NULL || token == NULL) {
        return 0;
    }
    
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
        token->value[0] = '\0';
        token->line = *line;
        token->column = *col;
        return 1;
    }
    
    if (parse_number(input, pos, *line, *col, token)) return 1;
    if (