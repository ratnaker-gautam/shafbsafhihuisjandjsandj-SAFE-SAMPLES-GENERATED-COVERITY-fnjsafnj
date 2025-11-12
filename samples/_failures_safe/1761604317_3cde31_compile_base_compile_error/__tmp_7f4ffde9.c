//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int i = 0;
    int has_dot = 0;
    int input_len = strlen(input);
    
    while (*pos < input_len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = input[*pos];
        }
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int i = 0;
    int input_len = strlen(input);
    
    if (*pos < input_len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int i = 0;
    int input_len = strlen(input);
    
    if (*pos < input_len && is_operator_char(input[*pos])) {
        buffer[i++] = input[*pos];
        (*pos)++;
        
        if (*pos < input_len && input[*pos] == '=' && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = input[*pos];
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int i = 0;
    int input_len = strlen(input);
    
    if (*pos < input_len && input[*pos] == '"') {
        (*pos)++;
        while (*pos < input_len && input[*pos] != '"' && input[*pos] != '\0') {
            if (i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = input[*pos];
            }
            (*pos)++;
        }
        if (*pos < input_len && input[*pos] == '"') {
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
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
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_operator(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_string(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    return token_count;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENT