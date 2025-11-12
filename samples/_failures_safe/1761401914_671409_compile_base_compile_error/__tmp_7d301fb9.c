//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    size_t input_len = strlen(input);
    
    while (*pos < input_len && isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    size_t input_len = strlen(input);
    
    if (*pos < input_len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        buffer[len++] = input[(*pos)++];
        
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    size_t input_len = strlen(input);
    
    if (*pos < input_len && is_operator_char(input[*pos])) {
        buffer[len++] = input[(*pos)++];
        
        if (*pos < input_len && is_operator_char(input[*pos])) {
            if (len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = input[(*pos)++];
            }
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    size_t input_len = strlen(input);
    
    if (*pos < input_len && input[*pos] == '"') {
        buffer[len++] = input[(*pos)++];
        
        while (*pos < input_len && input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LEN - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        if (*pos < input_len && input[*pos] == '"') {
            buffer[len++] = input[(*pos)++];
            buffer[len] = '\0';
            return 1;
        }
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    size_t input_len = strlen(input);
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (pos < input_len && token_count < max_tokens) {
        while (pos < input_len && isspace(input[pos])) pos++;
        
        if (pos >= input_len) break;
        
        char buffer[MAX_TOKEN_LEN];
        int success = 0;
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            success = 1;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            success = 1;
        } else if (parse_operator(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            success = 1;
        } else if (parse_string(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            success = 1;
        } else {
            tokens[token_count].type = TOKEN_INVALID;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            success = 1;
        }
        
        if (success) {
            token_count++;
        }