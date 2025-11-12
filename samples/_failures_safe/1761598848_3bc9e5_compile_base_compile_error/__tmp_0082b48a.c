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
    TOKEN_ERROR
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

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) {
                return -1;
            }
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) {
        return -1;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 0;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    (*pos)++;
    int start = *pos;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
        }
        if (input[*pos] != '\0') {
            (*pos)++;
        }
    }
    
    if (input[*pos] != '"') {
        return -1;
    }
    
    int len = *pos - start;
    if (len < 0 || len >= MAX_TOKEN_LEN) {
        return -1;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    (*pos)++;
    return 0;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) {
        return -1;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    return 0;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    while (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len <= 0 || len >= MAX_TOKEN_LEN) {
        return -1;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int col = 1;
    int token_count = 0;
    
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
        int result = -1;
        
        if (isdigit(input[pos])) {
            result = parse_number(input, &pos, line, col, &token);
        } else if (input[pos] == '"') {
            result = parse_string(input, &pos, line, col, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            result = parse_identifier(input, &pos, line, col, &token);
        } else if (is_operator_char(input[pos])) {
            result = parse_operator(input, &pos, line, col, &token);
        }
        
        if (result == 0) {
            if (token_count < max_tokens) {
                tokens[token_count] = token;
                token_count++;
                col += (int)strlen(token.value);
            }
        } else {
            if (token_count < max_tokens) {
                token.type = TOKEN_ERROR