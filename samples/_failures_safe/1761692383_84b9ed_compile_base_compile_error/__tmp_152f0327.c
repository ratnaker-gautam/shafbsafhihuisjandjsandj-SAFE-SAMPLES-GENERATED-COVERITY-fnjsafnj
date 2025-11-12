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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!is_valid_identifier_start(input[*pos])) return 0;
    
    (*pos)++;
    while (is_valid_identifier_char(input[*pos])) {
        (*pos)++;
    }
    
    token->type = TOKEN_IDENTIFIER;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    (*pos)++;
    while (is_operator_char(input[*pos])) {
        (*pos)++;
    }
    
    token->type = TOKEN_OPERATOR;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\') (*pos)++;
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        token->type = TOKEN_STRING;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

const char *token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (parse_number(input, &pos, &tokens[token_count]) ||
            parse_string(input, &pos, &tokens[token_count]) ||
            parse_identifier(input, &pos, &tokens[token_count]) ||
            parse_operator(input, &pos, &tokens[token_count])) {
            
            column += (int)strlen(tokens[token_count].value);
            token_count++;
        } else {
            tokens[token_count].type = TOKEN_UNKNOWN;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            column++;
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token