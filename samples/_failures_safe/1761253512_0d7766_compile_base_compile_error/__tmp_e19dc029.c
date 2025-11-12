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

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int start = *pos;
    
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (*pos >= len) return 0;
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < len) {
        char c = input[*pos];
        if (!isalnum(c) && c != '_') break;
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        char combo[3] = {input[start], input[*pos], '\0'};
        if (strcmp(combo, "==") == 0 || strcmp(combo, "!=") == 0 || 
            strcmp(combo, "<=") == 0 || strcmp(combo, ">=") == 0 ||
            strcmp(combo, "&&") == 0 || strcmp(combo, "||") == 0) {
            (*pos)++;
        }
    }
    
    int length = *pos - start;
    if (length > 31) length = 31;
    
    strncpy(token->value, input + start, length);
    token->value[length] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void get_next_token(const char *input, int *pos, int len, struct Token *token, int line, int column) {
    token->line = line;
    token->column = column;
    
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            token->line++;
            token->column = 1;
        } else {
            token->column++;
        }
        (*pos)++;
    }
    
    if (*pos >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return;
    }
    
    if (parse_number(input, pos, len, token)) return;
    if (parse_string(input, pos, len, token)) return;
    if (parse_identifier(input, pos, len, token)) return;
    if (parse_operator(input, pos, len, token)) return;
    
    token->type = TOKEN_INVALID;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFI