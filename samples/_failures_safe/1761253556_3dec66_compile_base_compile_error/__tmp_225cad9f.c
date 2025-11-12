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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int column) {
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
    token->column = column;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int column) {
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
    token->column = column;
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int column) {
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
    token->column = column;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int column) {
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
        
        int start_pos = pos;
        int start_column = column;
        
        if (parse_number(input, &pos, &tokens[token_count], line, start_column) ||
            parse_identifier(input, &pos, &tokens[token_count], line, start_column) ||
            parse_operator(input, &pos, &tokens[token_count], line, start_column) ||
            parse_string(input, &pos, &tokens[token_count], line, start_column)) {
            
            token_count++;
            column += (pos - start_pos);
            continue;
        }
        
        tokens[token_count].type = TOKEN_UNKNOWN;
        tokens[token_count].value[0] = input[pos];
        tokens[token_count].value[1] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
        pos++;
        column++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value