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
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_identifier(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    (*pos)++;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_operator(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    (*pos)++;
    if (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') {
            (*pos)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    (*pos)++;
    
    if (*pos - start > MAX_TOKEN_LEN - 1) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    return 1;
}

void skip_whitespace(const char *input, int *pos, int *line, int *col) {
    while (input[*pos] != '\0') {
        if (input[*pos] == ' ' || input[*pos] == '\t') {
            (*pos)++;
            (*col)++;
        } else if (input[*pos] == '\n') {
            (*pos)++;
            (*line)++;
            *col = 1;
        } else {
            break;
        }
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int col = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        skip_whitespace(input, &pos, &line, &col);
        
        if (input[pos] == '\0') break;
        
        int current_col = col;
        
        if (parse_number(input, &pos, line, current_col, &tokens[token_count]) ||
            parse_identifier(input, &pos, line, current_col, &tokens[token_count]) ||
            parse_operator(input, &pos, line, current_col, &tokens[token_count]) ||
            parse_string(input, &pos, line, current_col, &tokens[token_count])) {
            token_count++;
            col += (pos - (col - 1));
            continue;
        }
        
        tokens[token_count].type = TOKEN_ERROR;
        tokens[token_count].value[0] = input[pos];
        tokens[token_count].value[1] = '\