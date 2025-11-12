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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        (*pos)++;
        if (*pos < len && is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        while (*pos < len && input[*pos] != '"') {
            if (input[*pos] == '\\' && *pos + 1 < len) {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        if (*pos < len && input[*pos] == '"') {
            (*pos)++;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

void skip_whitespace(const char *input, int *pos, int len, int *line, int *column) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, int len, int *line, int *column, struct Token *token) {
    skip_whitespace(input, pos, len, line, column);
    
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    token->line = *line;
    token->column = *column;
    
    if (isdigit(input[*pos])) {
        if (parse_number(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        if (parse_identifier(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
    } else if (is_operator_char(input[*pos])) {
        if (parse_operator(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
    } else if (input[*pos] == '"') {
        if (parse_string(input, pos, len, token)) {
            *column += (int)strlen(token->value);
            return 1;
        }
    }
    
    token->type = TOKEN_ERROR;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)