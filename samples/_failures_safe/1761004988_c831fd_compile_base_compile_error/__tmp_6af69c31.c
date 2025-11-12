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
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input == NULL || pos == NULL || buffer == NULL) return 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input == NULL || pos == NULL || buffer == NULL) return 0;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[len++] = input[(*pos)++];
        
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    *pos = start_pos;
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input == NULL || pos == NULL || buffer == NULL) return 0;
    
    if (input[*pos] == '"') {
        (*pos)++;
        
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            buffer[len] = '\0';
            return 1;
        }
    }
    
    *pos = start_pos;
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input == NULL || pos == NULL || buffer == NULL) return 0;
    
    if (is_operator_char(input[*pos])) {
        buffer[len++] = input[(*pos)++];
        
        if (is_operator_char(input[*pos])) {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = input[(*pos)++];
            }
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    *pos = start_pos;
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int token_count = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
        } else {
            token->type = TOKEN_UNKNOWN;
            if (pos < (int)strlen(input)) {
                token->value[0] = input[pos++];
                token->value[1] = '\0';
            } else {
                token->value[0] = '\0';
            }
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_