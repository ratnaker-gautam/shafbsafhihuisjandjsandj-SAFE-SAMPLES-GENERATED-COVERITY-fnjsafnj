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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    while (input[start_pos] != '\0' && isdigit(input[start_pos])) {
        if (len >= MAX_TOKEN_LEN - 1) break;
        buffer[len++] = input[start_pos];
        start_pos++;
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        *pos = start_pos;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input[start_pos] == '\0' || !is_valid_identifier_start(input[start_pos])) return 0;
    
    buffer[len++] = input[start_pos];
    start_pos++;
    
    while (input[start_pos] != '\0' && is_valid_identifier_char(input[start_pos])) {
        if (len >= MAX_TOKEN_LEN - 1) break;
        buffer[len++] = input[start_pos];
        start_pos++;
    }
    
    buffer[len] = '\0';
    *pos = start_pos;
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input[start_pos] == '\0' || !is_operator_char(input[start_pos])) return 0;
    
    buffer[len++] = input[start_pos];
    start_pos++;
    
    if (input[start_pos] != '\0' && is_operator_char(input[start_pos])) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = input[start_pos];
            start_pos++;
        }
    }
    
    buffer[len] = '\0';
    *pos = start_pos;
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input[start_pos] == '\0' || input[start_pos] != '"') return 0;
    
    buffer[len++] = input[start_pos];
    start_pos++;
    
    while (input[start_pos] != '\0' && input[start_pos] != '"') {
        if (len >= MAX_TOKEN_LEN - 1) break;
        buffer[len++] = input[start_pos];
        start_pos++;
    }
    
    if (input[start_pos] == '"') {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = input[start_pos];
            start_pos++;
        }
    }
    
    buffer[len] = '\0';
    *pos = start_pos;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
            continue;
        }
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            token_count++;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else {
            token->type = TOKEN_UNKNOWN;
            if (pos < (int)strlen(input)) {
                token->value[0] = input[pos];
                token->value[1] = '\0';
                pos++;
            } else {
                token->value[0] = '\0';
            }
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print