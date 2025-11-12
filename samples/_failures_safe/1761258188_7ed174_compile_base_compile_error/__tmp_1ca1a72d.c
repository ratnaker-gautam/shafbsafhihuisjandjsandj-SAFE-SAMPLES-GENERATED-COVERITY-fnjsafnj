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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && is_operator_char(input[*pos])) {
        (*pos)++;
        if (*pos < len && is_operator_char(input[*pos])) {
            (*pos)++;
        }
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && input[*pos] == '"') {
        (*pos)++;
        while (*pos < len && input[*pos] != '"') {
            if (input[*pos] == '\\' && *pos + 1 < len) {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        if (*pos < len && input[*pos] == '"') {
            (*pos)++;
        }
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int len = strlen(input);
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (pos >= len) break;
        
        struct Token token = {0};
        token.line = line;
        token.column = column;
        
        int parsed = 0;
        if (input[pos] == '"') {
            parsed = parse_string(input, &pos, len, &token);
        } else if (isdigit(input[pos])) {
            parsed = parse_number(input, &pos, len, &token);
        } else if (is_operator_char(input[pos])) {
            parsed = parse_operator(input, &pos, len, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            parsed = parse_identifier(input, &pos, len, &token);
        }
        
        if (parsed) {
            if (token_count < max_tokens) {
                tokens[token_count] = token;
                token_count++;
                column += (int)strlen(token.value);
            }
        } else {
            pos++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].