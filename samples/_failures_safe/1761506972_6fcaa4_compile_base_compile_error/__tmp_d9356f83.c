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
    if (length > 31) length = 31;
    
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
    while (input[*pos] != '\0' && 
           (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
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
        int length = *pos - start;
        if (length > 31) length = 31;
        
        strncpy(token->value, &input[start], length);
        token->value[length] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (input[*pos] != '\0' && is_operator_char(input[*pos]) && 
        ((input[start] == '&' && input[*pos] == '&') ||
         (input[start] == '|' && input[*pos] == '|') ||
         (input[start] == '=' && input[*pos] == '=') ||
         (input[start] == '!' && input[*pos] == '=') ||
         (input[start] == '<' && input[*pos] == '=') ||
         (input[start] == '>' && input[*pos] == '='))) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    skip_whitespace(input, pos);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER