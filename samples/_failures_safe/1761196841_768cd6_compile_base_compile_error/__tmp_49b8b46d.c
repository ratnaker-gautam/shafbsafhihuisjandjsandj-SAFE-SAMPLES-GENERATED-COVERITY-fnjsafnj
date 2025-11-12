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
    TOKEN_UNKNOWN
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
    if (input == NULL || pos == NULL || token == NULL) return 0;
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
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int len = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"' && len < 31) {
        token->value[len++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        (*pos)++;
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int len = 0;
    
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_') && len < 31) {
        token->value[len++] = input[*pos];
        (*pos)++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL) return 0;
    if (!is_operator_char(input[*pos])) return 0;
    
    int len = 0;
    
    while (input[*pos] != '\0' && is_operator_char(input[*pos]) && len < 31) {
        token->value[len++] = input[*pos];
        (*pos)++;
    }
    
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    if (input == NULL || pos == NULL || token == NULL || line == NULL || column == NULL) return 0;
    
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
            *column += (int)strlen(token->value) + 2;
            return 1;
        }
        
        if (parse_identifier(input, pos, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
        
        if (parse_operator(input, pos, token)) {
            *column += (int)strlen(token->value);
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
    token->line = *line;
    token->column = *column;
    return 1;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TO