//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    while (*p && (isdigit(*p) || *p == '.')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    token->type = TOKEN_NUMBER;
    *input = p;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!isalpha(*p) && *p != '_') return 0;
    
    while (*p && (isalnum(*p) || *p == '_')) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = p;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (!is_operator_char(*p)) return 0;
    
    while (*p && is_operator_char(*p)) {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    token->type = TOKEN_OPERATOR;
    *input = p;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p != '"') return 0;
    p++;
    
    while (*p && *p != '"') {
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        *dest++ = *p++;
        len++;
    }
    
    if (*p != '"') return 0;
    p++;
    *dest = '\0';
    token->type = TOKEN_STRING;
    *input = p;
    return 1;
}

int get_next_token(const char **input, Token *token) {
    const char *p = *input;
    
    while (*p && isspace(*p)) p++;
    
    if (!*p) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        *input = p;
        return 1;
    }
    
    if (parse_number(&p, token)) {
        *input = p;
        return 1;
    }
    
    if (parse_identifier(&p, token)) {
        *input = p;
        return 1;
    }
    
    if (parse_operator(&p, token)) {
        *input = p;
        return 1;
    }
    
    if (parse_string(&p, token)) {
        *input = p;
        return 1;
    }
    
    token->type = TOKEN_ERROR;
    token->value[0] = *p;
    token->value[1] = '\0';
    p++;
    *input = p;
    return 0;
}

void tokenize(const char *input, Token *tokens, int *token_count) {
    const char *p = input;
    Token token;
    *token_count = 0;
    
    while (*p && *token_count < MAX_TOKENS) {
        if (!get_next_token(&p, &token)) {
            if (token.type == TOKEN_ERROR) {
                tokens[*token_count].type = TOKEN_ERROR;
                strncpy(tokens[*token_count].value, token.value, MAX_TOKEN_LEN - 1);
                tokens[*token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                (*token_count)++;
            }
            continue;
        }
        
        if (token.type == TOKEN_EOF) break;
        
        tokens[*token_count].type = token.type;
        strncpy(tokens[*token_count].value, token.value, MAX_TOKEN_LEN - 1);
        tokens[*token_count].value[MAX_TOKEN_LEN - 1] = '\0';
        (*token_count)++;
    }
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";