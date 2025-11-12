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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    while (*pos < input_len && isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    if (*pos < input_len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
        
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    if (*pos < input_len && is_operator_char(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
        
        if (*pos < input_len && is_operator_char(input[*pos])) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    int input_len = strlen(input);
    
    if (*pos < input_len && input[*pos] == '"') {
        (*pos)++;
        
        while (*pos < input_len && input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            buffer[len++] = input[(*pos)++];
        }
        
        if (*pos < input_len && input[*pos] == '"') {
            (*pos)++;
            buffer[len] = '\0';
            return 1;
        }
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int input_len = strlen(input);
    
    while (pos < input_len && token_count < max_tokens) {
        while (pos < input_len && isspace(input[pos])) pos++;
        
        if (pos >= input_len) break;
        
        char buffer[MAX_TOKEN_LEN];
        struct Token *token = &tokens[token_count];
        
        if (parse_number(input, &pos, buffer)) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_identifier(input, &pos, buffer)) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_operator(input, &pos, buffer)) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_string(input, &pos, buffer)) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else {
            token->type = TOKEN_UNKNOWN;
            if (MAX_TOKEN_LEN > 1) {
                token->value[0] = input[pos];
                token->value[1] = '\0';
            }
            pos++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(