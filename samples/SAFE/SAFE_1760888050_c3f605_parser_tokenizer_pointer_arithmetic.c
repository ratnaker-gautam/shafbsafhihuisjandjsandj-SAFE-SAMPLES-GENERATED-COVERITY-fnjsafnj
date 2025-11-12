//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

TokenType classify_token(const char *value) {
    if (value[0] == '\0') return TOKEN_INVALID;
    
    if (isdigit(value[0])) {
        char *end;
        strtol(value, &end, 10);
        if (*end == '\0') return TOKEN_NUMBER;
        return TOKEN_INVALID;
    }
    
    if (value[0] == '"' && value[strlen(value)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (is_operator_char(value[0]) && strlen(value) == 1) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha(value[0]) || value[0] == '_') {
        for (const char *p = value + 1; *p; p++) {
            if (!isalnum(*p) && *p != '_') return TOKEN_INVALID;
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_INVALID;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    char *current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        if (!*current) break;
        
        Token *token = tokens + token_count;
        char *token_start = current;
        
        if (isdigit(*current)) {
            while (*current && isdigit(*current)) current++;
        } else if (isalpha(*current) || *current == '_') {
            while (*current && (isalnum(*current) || *current == '_')) current++;
        } else if (*current == '"') {
            current++;
            while (*current && *current != '"') current++;
            if (*current == '"') current++;
        } else if (is_operator_char(*current)) {
            current++;
        } else {
            token->type = TOKEN_INVALID;
            size_t len = current - token_start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token->value, token_start, len);
            token->value[len] = '\0';
            token_count++;
            break;
        }
        
        size_t token_len = current - token_start;
        if (token_len >= MAX_TOKEN_LEN) {
            token_len = MAX_TOKEN_LEN - 1;
        }
        
        strncpy(token->value, token_start, token_len);
        token->value[token_len] = '\0';
        token->type = classify_token(token->value);
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_INVALID: printf("INVALID"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value=\"%s\"\n", tokens[i].value);
    }
    
    return 0;
}