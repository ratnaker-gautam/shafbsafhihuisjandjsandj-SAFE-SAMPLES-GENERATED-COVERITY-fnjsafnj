//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_decimal = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_decimal) {
            has_decimal = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, input + start, len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (!is_valid_identifier_start(input[*pos])) {
        return 0;
    }
    
    (*pos)++;
    while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, input + start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (!is_operator_char(input[*pos])) {
        return 0;
    }
    
    (*pos)++;
    if (input[*pos] != '\0' && is_operator_char(input[*pos])) {
        if ((input[start] == '&' && input[*pos] == '&') ||
            (input[start] == '|' && input[*pos] == '|') ||
            (input[start] == '=' && input[*pos] == '=') ||
            (input[start] == '!' && input[*pos] == '=') ||
            (input[start] == '<' && input[*pos] == '=') ||
            (input[start] == '>' && input[*pos] == '=')) {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, input + start, len);
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = col;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input[*pos] != '"') {
        return 0;
    }
    
    int start = *pos;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        int len = *pos - start;
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(token->value, input + start, len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
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
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                col = 1;
            } else