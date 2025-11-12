//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int len = strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos >= len || input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) (*pos)++;
        (*pos)++;
    }
    
    if (*pos < len && input[*pos] == '"') {
        int token_len = *pos - start - 1;
        if (token_len >= 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start + 1], token_len);
            token->value[token_len] = '\0';
            token->type = TOKEN_STRING;
            (*pos)++;
            return 1;
        }
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos >= len || (!isalpha(input[*pos]) && input[*pos] != '_')) return 0;
    
    int start = *pos;
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos >= len || !is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    while (*pos < len && is_operator_char(input[*pos])) {
        (*pos)++;
        if (*pos - start >= 2) break;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        struct Token token;
        int success = 0;
        
        if (parse_number(input, &pos, &token)) success = 1;
        else if (parse_string(input, &pos, &token)) success = 1;
        else if (parse_identifier(input, &pos, &token)) success = 1;
        else if (parse_operator(input, &pos, &token)) success = 1;
        
        if (success) {
            if (token_count < max_tokens) {
                tokens[token_count++] = token;
            }
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
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
        case TOKEN_EOF: printf("EOF");