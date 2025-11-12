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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, struct Token *token, int line, int column) {
    int i = 0;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = input[*pos];
            }
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (i > 0) {
        token->value[i] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token, int line, int column) {
    if (input[*pos] != '"') return 0;
    
    int i = 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token, int line, int column) {
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    int i = 0;
    while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (i > 0) {
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token, int line, int column) {
    if (!is_operator_char(input[*pos])) return 0;
    
    int i = 0;
    while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 2) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = input[*pos];
        }
        (*pos)++;
    }
    
    if (i > 0) {
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = line;
        token->column = column;
        return 1;
    }
    return 0;
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
        int found = 0;
        
        if (parse_number(input, &pos, &token, line, column)) found = 1;
        else if (parse_string(input, &pos, &token, line, column)) found = 1;
        else if (parse_identifier(input, &pos, &token, line, column)) found = 1;
        else if (parse_operator(input, &pos, &token, line, column)) found = 1;
        
        if (found) {
            tokens[token_count++] = token;
            column += (int)strlen(token.value);
        } else {
            struct Token error_token;
            error_token.type = TOKEN_ERROR;
            snprintf(error_token.value, MAX_TOKEN_LEN, "%c", input[pos]);
            error_token.line = line;
            error_token.column = column;
            tokens[token_count++] = error_token;
            pos