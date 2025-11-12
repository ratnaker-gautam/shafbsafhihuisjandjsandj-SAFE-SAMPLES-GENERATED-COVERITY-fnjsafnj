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

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

TokenType classify_token(const char *str) {
    if (isdigit(*str)) return TOKEN_NUMBER;
    if (isalpha(*str)) return TOKEN_IDENTIFIER;
    if (is_operator_char(*str)) return TOKEN_OPERATOR;
    if (*str == '"') return TOKEN_STRING;
    return TOKEN_INVALID;
}

int parse_number(const char **input, Token *token) {
    char *end;
    long num = strtol(*input, &end, 10);
    if (end == *input) return 0;
    if (num < -2147483647 || num > 2147483647) return 0;
    
    size_t len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    while (**input && is_operator_char(**input)) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    (*input)++;
    
    const char *start = *input;
    while (**input && **input != '"') {
        (*input)++;
    }
    
    if (**input != '"') return 0;
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens - 1) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        TokenType type = classify_token(input);
        int success = 0;
        
        switch (type) {
            case TOKEN_NUMBER:
                success = parse_number(&input, &token);
                break;
            case TOKEN_IDENTIFIER:
                success = parse_identifier(&input, &token);
                break;
            case TOKEN_OPERATOR:
                success = parse_operator(&input, &token);
                break;
            case TOKEN_STRING:
                success = parse_string(&input, &token);
                break;
            default:
                token.type = TOKEN_INVALID;
                token.value[0] = *input;
                token.value[1] = '\0';
                input++;
                success = 1;
                break;
        }
        
        if (success) {
            tokens[count++] = token;
        } else {
            break;
        }
    }
    
    tokens[count].type = TOKEN_EOF;
    tokens[count].value[0] = '\0';
    return count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("INVALID"); break;
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
    if (len >