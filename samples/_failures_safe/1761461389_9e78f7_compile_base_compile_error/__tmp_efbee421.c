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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
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
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
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

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
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

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    int start_pos = *pos;
    
    if (input[*pos] != '"') return 0;
    
    buffer[len++] = input[(*pos)++];
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        buffer[len++] = input[(*pos)++];
    }
    
    if (input[*pos] == '"') {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        buffer[len++] = input[(*pos)++];
        buffer[len] = '\0';
        return 1;
    }
    
    *pos = start_pos;
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        char buffer[MAX_TOKEN_LEN];
        int success = 0;
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_operator(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_string(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token