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
    char value[64];
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

int parse_number(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) {
                return 0;
            }
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_identifier(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    
    while (is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] != '"') {
        return 0;
    }
    
    int length = *pos - start - 1;
    if (length > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start + 1], length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = column;
    (*pos)++;
    return 1;
}

int parse_operator(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    
    while (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = column;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    while (input[*pos] != '\0') {
        char current = input[*pos];
        
        if (isspace(current)) {
            if (current == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        if (isdigit(current)) {
            return parse_number(input, pos, *line, *column, token);
        }
        
        if (is_valid_identifier_start(current)) {
            return parse_identifier(input, pos, *line, *column, token);
        }
        
        if (current == '"') {
            return parse_string(input, pos, *line, *column, token);
        }
        
        if (is_operator_char(current)) {
            return parse_operator(input, pos, *line, *column, token);
        }
        
        token->type = TOKEN_INVALID;
        token->value[0] = current;
        token->value[1] = '\0';
        token->line = *line;
        token->column = *column;
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

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d