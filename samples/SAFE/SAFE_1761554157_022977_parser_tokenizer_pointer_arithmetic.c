//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    if (value[0] == '"') return TOKEN_STRING;
    return TOKEN_INVALID;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    char *current = input;
    
    while (*current && token_count < max_tokens) {
        skip_whitespace(&current);
        if (!*current) break;
        
        Token *token = tokens + token_count;
        char *start = current;
        
        if (isalpha(*current) || *current == '_') {
            while (*current && (isalnum(*current) || *current == '_')) {
                current++;
            }
        } else if (isdigit(*current)) {
            while (*current && isdigit(*current)) {
                current++;
            }
        } else if (is_operator_char(*current)) {
            while (*current && is_operator_char(*current)) {
                current++;
            }
        } else if (*current == '"') {
            current++;
            while (*current && *current != '"') {
                current++;
            }
            if (*current == '"') current++;
        } else {
            token->type = TOKEN_INVALID;
            strncpy(token->value, start, 1);
            token->value[1] = '\0';
            current++;
            token_count++;
            continue;
        }
        
        size_t len = current - start;
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        
        strncpy(token->value, start, len);
        token->value[len] = '\0';
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
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_INVALID: printf("INVALID"); break;
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
    }
    
    return 0;
}