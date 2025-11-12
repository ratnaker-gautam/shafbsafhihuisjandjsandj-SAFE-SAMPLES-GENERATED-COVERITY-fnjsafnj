//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        token->type = TOKEN_NUMBER;
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        token->type = TOKEN_STRING;
        int len = *pos - start - 2;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        strncpy(token->value, &input[start + 1], len);
        token->value[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    token->type = TOKEN_IDENTIFIER;
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    token->type = TOKEN_OPERATOR;
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    return 1;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens, int *num_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *num_tokens = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && *num_tokens < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        tokens[*num_tokens].line = line;
        tokens[*num_tokens].column = column;
        
        if (parse_number(input, &pos, &tokens[*num_tokens]) ||
            parse_string(input, &pos, &tokens[*num_tokens]) ||
            parse_identifier(input, &pos, &tokens[*num_tokens]) ||
            parse_operator(input, &pos, &tokens[*num_tokens])) {
            
            column += strlen(tokens[*num_tokens].value);
            (*num_tokens)++;
        } else {
            tokens[*num_tokens].type = TOKEN_UNKNOWN;
            tokens[*num_tokens].value[0] = input[pos];
            tokens[*num_tokens].value[1] = '\0';
            pos