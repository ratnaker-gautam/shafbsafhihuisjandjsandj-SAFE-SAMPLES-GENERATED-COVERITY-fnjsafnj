//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LENGTH 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LENGTH];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int has_decimal = 0;
    
    while (input[*pos] && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_decimal) break;
            has_decimal = 1;
        }
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length >= MAX_TOKEN_LENGTH) return 0;
    
    strncpy(buffer, &input[start], length);
    buffer[length] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    (*pos)++;
    
    while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length >= MAX_TOKEN_LENGTH) return 0;
    
    strncpy(buffer, &input[start], length);
    buffer[length] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int start = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    buffer[0] = input[*pos];
    (*pos)++;
    
    if (input[*pos] && is_operator_char(input[*pos]) && 
        ((buffer[0] == '&' && input[*pos] == '&') ||
         (buffer[0] == '|' && input[*pos] == '|') ||
         (buffer[0] == '=' && input[*pos] == '=') ||
         (buffer[0] == '!' && input[*pos] == '=') ||
         (buffer[0] == '<' && input[*pos] == '=') ||
         (buffer[0] == '>' && input[*pos] == '='))) {
        buffer[1] = input[*pos];
        buffer[2] = '\0';
        (*pos)++;
    } else {
        buffer[1] = '\0';
    }
    
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    int start = ++(*pos);
    int length = 0;
    
    while (input[*pos] && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1]) {
            (*pos) += 2;
            length += 2;
        } else {
            (*pos)++;
            length++;
        }
        
        if (length >= MAX_TOKEN_LENGTH - 1) return 0;
    }
    
    if (!input[*pos]) return 0;
    
    strncpy(buffer, &input[start], length);
    buffer[length] = '\0';
    (*pos)++;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        if (!input[pos]) break;
        
        struct Token *token = &tokens[token_count];
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            token_count++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
        } else {
            printf("Error: Invalid character '%c' at position %d\n", input[pos], pos);
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf