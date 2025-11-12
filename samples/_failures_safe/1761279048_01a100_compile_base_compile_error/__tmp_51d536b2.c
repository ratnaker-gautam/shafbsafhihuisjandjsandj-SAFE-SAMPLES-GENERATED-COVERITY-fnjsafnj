//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int col) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int col) {
    int start = *pos;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int col) {
    int start = *pos;
    
    if (is_operator_char(input[*pos])) {
        (*pos)++;
        if (is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int col) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int len = *pos - start;
        if (len < MAX_TOKEN_LEN) {
            token->type = TOKEN_STRING;
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->line = line;
            token->column = col;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int col = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
            pos++;
            continue;
        }
        
        struct Token token;
        int found = 0;
        
        if (parse_number(input, &pos, &token, line, col)) found = 1;
        else if (parse_string(input, &pos, &token, line, col)) found = 1;
        else if (parse_identifier(input, &pos, &token, line, col)) found = 1;
        else if (parse_operator(input, &pos, &token, line, col)) found = 1;
        
        if (found) {
            if (token_count < max_tokens) {
                tokens[token_count] = token;
                token_count++;
                col += (int)strlen(token.value);
            }
        } else {
            if (token_count < max_tokens) {
                token.type = TOKEN_UNKNOWN;