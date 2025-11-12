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
    TOKEN_INVALID
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) {
                return 0;
            }
            has_dot = 1;
        }
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_identifier(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    
    while (is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_operator(const char *input, int *pos, int line, int column, struct Token *token) {
    int start = *pos;
    
    while (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int column, struct Token *token) {
    if (input[*pos] != '"') {
        return 0;
    }
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = column;
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        struct Token token;
        int success = 0;
        
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, line, column, &token);
        } else if (is_valid_identifier_start(input[pos])) {
            success = parse_identifier(input, &pos, line, column, &token);
        } else if (is_operator_char(input[pos])) {
            success = parse_operator(input, &pos, line, column, &token);
        } else if (input[pos] == '"') {
            success = parse_string(input, &pos, line, column, &token);
        }
        
        if (success) {
            if (token_count < max_tokens) {
                tokens[token_count] = token;
                token_count++;
                column += strlen(token.value);
            }
        } else {
            if (token_count < max_tokens) {
                token.type = TOKEN_INVALID;
                token.value[0] = input[pos];
                token.value[1] = '\0';
                token.line = line;
                token.column = column;
                tokens[token_count]