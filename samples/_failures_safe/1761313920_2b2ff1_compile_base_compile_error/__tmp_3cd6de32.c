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
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos > start) {
        int len = *pos - start;
        if (len >= 64) return 0;
        
        token->type = TOKEN_NUMBER;
        memcpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] != '\0' && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= 64) return 0;
        
        token->type = TOKEN_IDENTIFIER;
        memcpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && (*pos - start) < 3) {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= 64) return 0;
        
        token->type = TOKEN_OPERATOR;
        memcpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input[*pos] == '"') {
        int start = *pos;
        (*pos)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            int len = *pos - start;
            if (len >= 64) return 0;
            
            token->type = TOKEN_STRING;
            memcpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->line = line;
            token->column = col;
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
            continue;
        }
        
        int current_pos = *pos;
        int current_line = *line;
        int current_col = *col;
        
        if (parse_number(input, pos, current_line, current_col, token)) return 1;
        *pos = current_pos;
        
        if (parse_string(input, pos, current_line, current_col, token)) return 1;
        *pos = current_pos;
        
        if (parse_identifier(input, pos, current_line, current_col, token)) return 1;
        *pos = current_pos;
        
        if (parse_operator(input, pos, current_line, current_col, token)) return 1;
        *pos = current_pos;
        
        token->type = TOKEN_UNKNOWN;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->line = current_line;
        token->column = current_col;
        (*pos)++;
        (*col)++;
        return 1;