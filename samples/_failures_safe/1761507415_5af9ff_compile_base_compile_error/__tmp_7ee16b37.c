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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, struct Token *token) {
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
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    int start = *pos;
    
    if (input[*pos] == '\0' || !is_valid_identifier_start(input[*pos])) return 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    int start = *pos;
    
    if (input[*pos] == '\0' || !is_operator_char(input[*pos])) return 0;
    (*pos)++;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
            if (input[*pos] == '\0') return 0;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int len = *pos - start;
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || column == NULL || token == NULL) return 0;
    
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
            *column += strlen(token->value);
            return 1;
        }
        
        if (parse_identifier(input, pos, token)) {
            *column += strlen(token->value);
            return 1;
        }
        
        if (parse_operator(input, pos, token)) {
            *column += strlen(token->value);
            return 1