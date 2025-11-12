//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        buffer[len++] = input[(*pos)++];
        
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (len >= MAX_TOKEN_LEN - 1) {
                *pos = start_pos;
                return 0;
            }
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return len > 0;
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input == NULL || pos == NULL || buffer == NULL) return 0;
    
    if (is_operator_char(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        buffer[len++] = input[(*pos)++];
        
        if (is_operator_char(input[*pos])) {
            if (len >= MAX_TOKEN_LEN - 1) {
                *pos = start_pos;
                return 0;
            }
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return len > 0;
    }
    
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input == NULL || pos == NULL || buffer == NULL) return 0;
    
    if (input[*pos] == '"') {
        (*pos)++;
        
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LEN - 1) {
                *pos = start_pos;
                return 0;
            }
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

void skip_whitespace(const char *input, int *pos) {
    if (input == NULL || pos == NULL) return;
    
    while (isspace(input[*pos])) {
        (*pos)++;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        
        if (input[pos] == '\0') break;
        
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
            if (token_count < max_tokens) {
                token->type = TOKEN_INVALID;
                if (MAX_TOKEN_LEN > 1) {
                    token->value[0] = input[pos];
                    token->value[1] = '\0';
                }
                token_count++;
                pos++;
            }