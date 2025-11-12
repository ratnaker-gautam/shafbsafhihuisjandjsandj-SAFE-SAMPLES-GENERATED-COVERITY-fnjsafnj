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
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] && (isdigit(input[*pos]) || input[*pos] == '.')) {
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
    
    if (input[*pos] && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
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
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1]) {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (input[*pos] != '"') return 0;
    
    int len = *pos - start + 1;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] && is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        if (!input[pos]) break;
        
        int current_column = column;
        struct Token token = {0};
        token.line = line;
        token.column = current_column;
        
        int found = 0;
        int old_pos = pos;
        
        if (parse_number(input, &pos, &token)) found = 1;
        else if (parse_string(input, &pos, &token)) found = 1;
        else if (parse_identifier(input, &pos, &token)) found = 1;
        else if (parse_operator(input, &pos, &token)) found = 1;
        
        if (!found) {
            pos = old_pos;
            token.value[0] = input[pos];
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            pos++;
        }
        
        tokens[token_count] = token;
        token_count++;
        
        for (int i = old_pos; i < pos; i++) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value