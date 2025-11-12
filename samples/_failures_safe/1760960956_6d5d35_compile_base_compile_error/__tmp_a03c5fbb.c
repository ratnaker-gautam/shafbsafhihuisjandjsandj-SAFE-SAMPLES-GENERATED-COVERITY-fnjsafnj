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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
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
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') (*pos)++;
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        token->type = TOKEN_STRING;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            (*pos)++;
        }
        
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        if (is_operator_char(input[*pos])) (*pos)++;
        
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    int line = 1;
    int column = 1;
    
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            (*pos)++;
            continue;
        }
        
        token->line = line;
        token->column = column;
        
        if (parse_number(input, pos, token)) return 1;
        if (parse_string(input, pos, token)) return 1;
        if (parse_identifier(input, pos, token)) return 1;
        if (parse_operator(input, pos, token)) return 1;
        
        token->type = TOKEN_UNKNOWN;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        column++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = line;
    token->column = column;
    return 1;
}

void tokenize(const char *input, struct Token *tokens, int *token_count) {
    int pos = 0;
    *token_count = 0;
    
    while (*token_count < MAX_TOKENS) {
        struct Token token;
        if (!get_next_token(input, &pos, &token)) break;
        
        if (token.type == TOKEN_EOF) {
            tokens[*token_count] = token;
            (*token_count)++;
            break;
        }
        
        if (token.type != TOKEN_UNKNOWN) {
            tokens[*token_count] = token;
            (*token_count)++;
        }
    }
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("