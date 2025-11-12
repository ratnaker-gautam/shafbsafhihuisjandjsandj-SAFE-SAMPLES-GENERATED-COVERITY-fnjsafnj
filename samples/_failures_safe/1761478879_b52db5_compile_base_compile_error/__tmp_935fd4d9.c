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
    int value_pos = 0;
    
    while (*pos < len && value_pos < 31) {
        char c = input[*pos];
        if (c == '"') {
            (*pos)++;
            token->value[value_pos] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        } else if (c == '\\') {
            (*pos)++;
            if (*pos < len) {
                token->value[value_pos++] = input[(*pos)++];
            }
        } else {
            token->value[value_pos++] = input[(*pos)++];
        }
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        char two_char[3] = {input[start], input[*pos], '\0'};
        if (strcmp(two_char, "==") == 0 || strcmp(two_char, "!=") == 0 ||
            strcmp(two_char, "<=") == 0 || strcmp(two_char, ">=") == 0 ||
            strcmp(two_char, "&&") == 0 || strcmp(two_char, "||") == 0) {
            strcpy(token->value, two_char);
            (*pos)++;
        } else {
            token->value[0] = input[start];
            token->value[1] = '\0';
        }
    } else {
        token->value[0] = input[start];
        token->value[1] = '\0';
    }
    
    token->type = TOKEN_OPERATOR;
    return 1;
}

int get_next_token(const char *input, int *pos, int len, int *line, int *column, struct Token *token) {
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
            int start_col = *column;
            if (parse_string(input, pos, len, token)) {
                *column = start_col + (*pos - (*pos - (int)strlen(token->value) - 2));
                return 1;
            }
        } else if (is_operator_char(c)) {