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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    if (input[*pos] == '.') {
        buffer[len++] = input[(*pos)++];
        while (isdigit(input[*pos])) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            buffer[len++] = input[(*pos)++];
        }
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    while (is_valid_identifier_char(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    buffer[len++] = input[(*pos)++];
    
    if (is_operator_char(input[*pos]) && len < MAX_TOKEN_LEN - 1) {
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (input[*pos] != '"') return 0;
    
    buffer[len++] = input[(*pos)++];
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        if (input[*pos] == '\\') {
            buffer[len++] = input[(*pos)++];
            if (input[*pos] == '\0') return 0;
        }
        buffer[len++] = input[(*pos)++];
    }
    
    if (input[*pos] == '"') {
        buffer[len++] = input[(*pos)++];
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        char buffer[MAX_TOKEN_LEN];
        
        if (parse_number(input, &pos, buffer)) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_string(input, &pos, buffer)) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_identifier(input, &pos, buffer)) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
        } else if (parse_operator(input, &pos, buffer)) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            pos