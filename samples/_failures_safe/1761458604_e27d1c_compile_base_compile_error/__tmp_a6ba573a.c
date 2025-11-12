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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos, int *line, int *col) {
    while (input[*pos] != '\0') {
        if (input[*pos] == ' ' || input[*pos] == '\t') {
            (*pos)++;
            (*col)++;
        } else if (input[*pos] == '\n') {
            (*pos)++;
            (*line)++;
            *col = 1;
        } else {
            break;
        }
    }
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int *col) {
    int i = 0;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        } else if (input[*pos] == '.' && !has_dot) {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            has_dot = 1;
            (*pos)++;
            (*col)++;
        } else {
            break;
        }
    }
    
    if (i > 0) {
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = *col - i;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int *col) {
    int i = 0;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        if (i < 31) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '\0') {
            if (isalnum(input[*pos]) || input[*pos] == '_') {
                if (i < 31) {
                    token->value[i++] = input[*pos];
                }
                (*pos)++;
                (*col)++;
            } else {
                break;
            }
        }
        
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = *col - i;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int *col) {
    int i = 0;
    
    if (is_operator_char(input[*pos])) {
        if (i < 31) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = *col - i;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int *col) {
    int i = 0;
    
    if (input[*pos] == '"') {
        if (i < 31) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"' && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        
        if (input[*pos] == '"') {
            if (i < 31) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
            token->value[i] = '\0';
            token->type = TOKEN_STRING;
            token->line = line;
            token->column = *col - i;
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token, int *line, int *col) {
    skip_whitespace(input, pos, line, col);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = *line;
        token->column = *col;
        return 1;
    }
    
    if (parse_number(input, pos, token, *line, col)) return 1;
    if