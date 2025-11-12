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
    
    while (*pos < len) {
        char c = input[*pos];
        if (isdigit(c)) {
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    int token_len = *pos - start;
    if (token_len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, &input[start], token_len);
    token->value[token_len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    int start = *pos;
    
    while (*pos < len && input[*pos] != '"') {
        (*pos)++;
    }
    
    if (*pos >= len) {
        return 0;
    }
    
    int token_len = *pos - start;
    if (token_len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, &input[start], token_len);
    token->value[token_len] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int parse_identifier(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        int token_len = *pos - start;
        if (token_len >= MAX_TOKEN_LEN) {
            return 0;
        }
        
        strncpy(token->value, &input[start], token_len);
        token->value[token_len] = '\0';
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
        
        int token_len = *pos - start;
        if (token_len >= MAX_TOKEN_LEN) {
            return 0;
        }
        
        strncpy(token->value, &input[start], token_len);
        token->value[token_len] = '\0';
        token->type = TOKEN_OPERATOR;
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
        char c = input[pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        int success = 0;
        int old_pos = pos;
        
        if (isdigit(c)) {
            success = parse_number(input, &pos, len, &tokens[token_count]);
        } else if (c == '"') {
            success = parse_string(input, &pos, len, &tokens[token_count]);
        } else if (is_operator_char(c)) {
            success = parse_operator(input, &pos, len, &tokens[token_count]);
        } else if (isalpha(c) || c == '_') {
            success = parse_identifier(input, &pos, len, &tokens[token_count]);
        }
        
        if (success) {
            column += (pos - old_pos);
            token_count++;
        } else {
            pos++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens