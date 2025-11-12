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
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    int has_dot = 0;
    
    while (*pos < len && i < MAX_TOKEN_LEN - 1) {
        char c = input[*pos];
        if (isdigit(c)) {
            buffer[i++] = c;
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            buffer[i++] = c;
            (*pos)++;
            has_dot = 1;
        } else {
            break;
        }
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        buffer[i++] = input[*pos];
        (*pos)++;
        
        while (*pos < len && i < MAX_TOKEN_LEN - 1) {
            char c = input[*pos];
            if (isalnum(c) || c == '_') {
                buffer[i++] = c;
                (*pos)++;
            } else {
                break;
            }
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        buffer[i++] = input[*pos];
        (*pos)++;
        
        if (*pos < len && is_operator_char(input[*pos]) && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = input[*pos];
            (*pos)++;
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        
        while (*pos < len && i < MAX_TOKEN_LEN - 1) {
            char c = input[*pos];
            if (c == '"') {
                (*pos)++;
                buffer[i] = '\0';
                return 1;
            } else if (c == '\\') {
                (*pos)++;
                if (*pos < len) {
                    buffer[i++] = input[*pos];
                    (*pos)++;
                }
            } else {
                buffer[i++] = c;
                (*pos)++;
            }
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int len = strlen(input);
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (pos >= len) break;
        
        char buffer[MAX_TOKEN_LEN];
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, len, buffer)) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_identifier(input, &pos, len, buffer)) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_operator(input, &pos, len, buffer)) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else if (parse_string(input, &pos, len, buffer)) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, MAX_TOKEN_LEN - 1);
            token->value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return