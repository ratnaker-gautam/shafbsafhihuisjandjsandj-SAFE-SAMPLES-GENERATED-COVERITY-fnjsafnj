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
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    size_t input_len = strlen(input);
    
    while (*pos < (int)input_len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    size_t input_len = strlen(input);
    
    if (*pos < (int)input_len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < (int)input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    size_t input_len = strlen(input);
    if (*pos >= (int)input_len || input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (*pos < (int)input_len && input[*pos] != '"') {
        if (input[*pos] == '\\') (*pos)++;
        (*pos)++;
    }
    
    if (*pos < (int)input_len && input[*pos] == '"') {
        (*pos)++;
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) return 0;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    size_t input_len = strlen(input);
    if (*pos >= (int)input_len || !is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    if (*pos < (int)input_len && is_operator_char(input[*pos])) (*pos)++;
    
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

void skip_whitespace(const char *input, int *pos) {
    size_t input_len = strlen(input);
    while (*pos < (int)input_len && isspace(input[*pos])) (*pos)++;
}

int get_next_token(const char *input, int *pos, int line, int column, struct Token *token) {
    size_t input_len = strlen(input);
    if (*pos >= (int)input_len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    skip_whitespace(input, pos);
    
    if (*pos >= (int)input_len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    token->line = line;
    token->column = column;
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    token->type = TOKEN_UNKNOWN;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

int tokenize(const char *input, struct Token tokens[], int max_tokens) {
    int pos = 0;
    int token_count