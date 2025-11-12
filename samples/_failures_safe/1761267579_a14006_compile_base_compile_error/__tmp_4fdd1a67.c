//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        strncpy(token->value, &input[start + 1], *pos - start - 1);
        token->value[*pos - start - 1] = '\0';
        (*pos)++;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    (*pos)++;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    (*pos)++;
    while (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
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
        
        struct Token token;
        token.line = line;
        token.column = column;
        
        int parsed = 0;
        parsed = parse_number(input, &pos, &token) ||
                parse_string(input, &pos, &token) ||
                parse_identifier(input, &pos, &token) ||
                parse_operator(input, &pos, &token);
        
        if (parsed) {
            tokens[token_count] = token;
            token_count++;
            column += (int)strlen(token.value);
        } else {
            token.type = TOKEN_UNKNOWN;
            token.value[0] = input[pos];
            token.value[1] = '\0';
            tokens[token_count] = token;
            token_count++;
            pos++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
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

void print_tokens(struct Token *tokens, int count) {
    printf("Tokens