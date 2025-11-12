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
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    token->value[i++] = input[*pos];
    (*pos)++;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (i >= MAX_TOKEN_LEN - 1) break;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    token->value[i++] = input[*pos];
    (*pos)++;
    
    if (is_operator_char(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) {
            token->value[i] = '\0';
            token->type = TOKEN_OPERATOR;
            token->line = line;
            token->column = column;
            return 1;
        }
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = column;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    
    if (input[*pos] != '"') return 0;
    
    token->value[i++] = input[*pos];
    (*pos)++;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (input[*pos] == '\\') {
            if (i >= MAX_TOKEN_LEN - 2) break;
            token->value[i++] = input[*pos];
            (*pos)++;
            if (input[*pos] == '\0') break;
        }
        if (i >= MAX_TOKEN_LEN - 1) break;
        token->value[i++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    token->value[i] = '\0';
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
        
        int start_column = column;
        
        if (parse_number(input, &pos, &tokens[token_count], line, start_column)) {
            token_count++;
        } else if (parse_identifier(input, &pos, &tokens[token_count], line, start_column)) {
            token_count++;
        } else if (parse_operator(input, &pos, &tokens[token_count], line, start_column)) {
            token_count++;
        } else if (parse_string(input, &pos, &tokens[token_count], line, start_column)) {
            token_count++;
        } else {
            pos++;
            column++;
            continue;
        }
        
        column += (int)strlen