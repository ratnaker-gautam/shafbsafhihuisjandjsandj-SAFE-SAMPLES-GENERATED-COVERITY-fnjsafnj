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
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos) {
    if (!input || !pos) return;
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
}

int parse_number(const char *input, int *pos, struct Token *token) {
    if (!input || !pos || !token) return 0;
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
        token->type = TOKEN_NUMBER;
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        if (len > 0) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
        }
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    if (!input || !pos || !token) return 0;
    int start = *pos;
    
    if (input[*pos] && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        token->type = TOKEN_IDENTIFIER;
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        if (len > 0) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
        }
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (!input || !pos || !token) return 0;
    int start = *pos;
    
    if (input[*pos] && is_operator_char(input[*pos])) {
        (*pos)++;
        while (input[*pos] && is_operator_char(input[*pos])) {
            (*pos)++;
        }
        
        token->type = TOKEN_OPERATOR;
        int len = *pos - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        if (len > 0) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
        }
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (!input || !pos || !token) return 0;
    if (input[*pos] == '"') {
        int start = *pos;
        (*pos)++;
        
        while (input[*pos] && input[*pos] != '"') {
            if (input[*pos] == '\\' && input[*pos + 1]) {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            token->type = TOKEN_STRING;
            int len = *pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            if (len > 0) {
                strncpy(token->value, &input[start], len);
                token->value[len] = '\0';
            }
            return 1;
        }
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    if (!input || !pos || !token) return 0;
    skip_whitespace(input, pos);
    
    if (!input[*pos]) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_string(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    return 0;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    if (token.type >= TOKEN_NUMBER && token.type <= TOKEN_