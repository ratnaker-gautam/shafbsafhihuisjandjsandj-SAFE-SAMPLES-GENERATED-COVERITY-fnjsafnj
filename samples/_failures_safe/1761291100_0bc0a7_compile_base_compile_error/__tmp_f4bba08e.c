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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void init_token(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

int parse_number(const char *input, int *pos, int line, int column, struct Token *token) {
    char buffer[64];
    int i = 0;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && i < 63) {
        if (isdigit(input[*pos])) {
            buffer[i++] = input[*pos];
        } else if (input[*pos] == '.' && !has_dot) {
            buffer[i++] = input[*pos];
            has_dot = 1;
        } else {
            break;
        }
        (*pos)++;
    }
    
    if (i > 0) {
        buffer[i] = '\0';
        init_token(token, TOKEN_NUMBER, buffer, line, column);
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int line, int column, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    char buffer[64];
    int i = 0;
    int start_pos = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && i < 63) {
        if (input[*pos] == '"') {
            buffer[i] = '\0';
            (*pos)++;
            init_token(token, TOKEN_STRING, buffer, line, column);
            return 1;
        }
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    
    *pos = start_pos;
    return 0;
}

int parse_identifier(const char *input, int *pos, int line, int column, struct Token *token) {
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    char buffer[64];
    int i = 0;
    
    while (input[*pos] != '\0' && i < 63 && is_valid_identifier_char(input[*pos])) {
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    
    if (i > 0) {
        buffer[i] = '\0';
        init_token(token, TOKEN_IDENTIFIER, buffer, line, column);
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int line, int column, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    char buffer[3] = {0};
    buffer[0] = input[*pos];
    int start_pos = *pos;
    (*pos)++;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        if ((buffer[0] == '&' && input[*pos] == '&') ||
            (buffer[0] == '|' && input[*pos] == '|') ||
            (buffer[0] == '=' && input[*pos] == '=') ||
            (buffer[0] == '!' && input[*pos] == '=') ||
            (buffer[0] == '<' && input[*pos] == '=') ||
            (buffer[0] == '>' && input[*pos] == '=')) {
            buffer[1] = input[*pos];
            (*pos)++;
        }
    }
    
    init_token(token, TOKEN_OPERATOR, buffer, line, column);
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
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
        
        int current_column = *column;
        int current_pos = *pos;
        
        if (parse_number(input, pos, *line, current_column, token)) {
            *column += (*pos - current_pos);
            return 1;
        }
        
        if (parse_string(input, pos, *line, current_column, token)) {
            *column += (*pos - current_pos);