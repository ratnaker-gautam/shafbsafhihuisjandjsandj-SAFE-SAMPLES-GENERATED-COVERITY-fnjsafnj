//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && strchr(DELIMITERS, **input)) {
        (*input)++;
    }
}

int parse_number(const char **input, Token *token) {
    size_t len = 0;
    const char *start = *input;
    
    while (**input && isdigit(**input)) {
        len++;
        (*input)++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len > 0) {
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    size_t len = 0;
    const char *start = *input;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        len++;
        (*input)++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            len++;
            (*input)++;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        strncpy(token->value, start, len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, Token *token) {
    if (**input && is_operator(**input)) {
        token->value[0] = **input;
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*input)++;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, Token *token) {
    size_t len = 0;
    const char *start = *input;
    
    if (**input == '"') {
        (*input)++;
        start = *input;
        
        while (**input && **input != '"') {
            len++;
            (*input)++;
            if (len >= MAX_TOKEN_LEN - 1) break;
        }
        
        if (**input == '"') {
            strncpy(token->value, start, len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            (*input)++;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input) {
    token_count = 0;
    
    while (*input && token_count < MAX_TOKENS) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        int parsed = 0;
        
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        
        if (parsed) {
            tokens[token_count++] = token;
        } else {
            token.value[0] = *input;
            token.value[1] = '\0';
            token.type = TOKEN_UNKNOWN;
            tokens[token_count++] = token;
            input++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "EOF");
    
    return token_count > 0;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

void print_tokens() {
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: Type: ", i);
        print_token_type(tokens[i].type);
        printf(", Value: '%s'\n", tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")]