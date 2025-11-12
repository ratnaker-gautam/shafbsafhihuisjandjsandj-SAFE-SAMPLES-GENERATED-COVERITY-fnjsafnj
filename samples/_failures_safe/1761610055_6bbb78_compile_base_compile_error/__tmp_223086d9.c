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
    TOKEN_EOF
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
    int start = *pos;
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    if (input[*pos] == '.' && isdigit(input[*pos + 1])) {
        buffer[len++] = input[(*pos)++];
        while (isdigit(input[*pos])) {
            if (len >= MAX_TOKEN_LEN - 1) return 0;
            buffer[len++] = input[(*pos)++];
        }
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    int len = 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        if (input[*pos] == '\\') {
            (*pos)++;
            if (input[*pos] == '\0') return 0;
        }
        buffer[len++] = input[(*pos)++];
    }
    
    if (input[*pos] != '"') return 0;
    (*pos)++;
    buffer[len] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int len = 0;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int len = 0;
    buffer[len++] = input[(*pos)++];
    
    if (is_operator_char(input[*pos]) && 
        ((buffer[0] == '&' && input[*pos] == '&') ||
         (buffer[0] == '|' && input[*pos] == '|') ||
         (buffer[0] == '=' && input[*pos] == '=') ||
         (buffer[0] == '!' && input[*pos] == '=') ||
         (buffer[0] == '<' && input[*pos] == '=') ||
         (buffer[0] == '>' && input[*pos] == '='))) {
        if (len < MAX_TOKEN_LEN - 1) {
            buffer[len++] = input[(*pos)++];
        }
    }
    
    buffer[len] = '\0';
    return len > 0;
}

void skip_whitespace(const char *input, int *pos) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        
        if (input[pos] == '\0') break;
        
        if (input[pos] == '\n') {
            line++;
            pos++;
            continue;
        }
        
        char buffer[MAX_TOKEN_LEN];
        int success = 0;
        
        if (parse_number(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_string(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].line = line;
            token_count++;
            success = 1;
        } else if (parse_identifier(input, &pos, buffer)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN -