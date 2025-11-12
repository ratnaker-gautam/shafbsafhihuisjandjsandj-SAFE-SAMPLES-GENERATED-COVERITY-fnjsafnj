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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token) {
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
    
    int length = *pos - start;
    if (length > 31 || length == 0) {
        return 0;
    }
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') {
        return 0;
    }
    
    (*pos)++;
    
    while (input[*pos] != '\0') {
        if (isalnum(input[*pos]) || input[*pos] == '_') {
            (*pos)++;
        } else {
            break;
        }
    }
    
    int length = *pos - start;
    if (length > 31) {
        return 0;
    }
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') {
        return 0;
    }
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0') {
        if (input[*pos] == '"') {
            (*pos)++;
            break;
        }
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 31) {
        return 0;
    }
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) {
        return 0;
    }
    
    int start = *pos;
    (*pos)++;
    
    if (input[*pos] == '=' && is_operator_char(input[start])) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 31) {
        return 0;
    }
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    token->line = *line;
    token->column = *column;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    token->line = *line;
    token->column = *column;
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        return;
    }
    
    int success = 0;
    int current_pos = *pos;
    
    do {
        if (parse_number(input, &current_pos, token)) {
            success = 1;
            break;
        }
        current_pos = *pos;
        
        if (parse_string(input, &current_pos, token)) {
            success = 1;
            break;
        }
        current_pos = *pos;
        
        if (parse_identifier(input, &current_pos, token)) {
            success = 1;
            break;
        }
        current_pos = *pos;
        
        if (parse_operator(input, &current_pos, token)) {
            success = 1;
            break;
        }
    } while (0);
    
    if (success) {
        int delta = current_pos - *pos;
        *pos = current_pos;
        *column += delta;
    } else {
        token->type = TOKEN_ERROR;
        token->value[0] = input[*pos];
        token->value