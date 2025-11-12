//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

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
        if (len >= MAX_TOKEN_LEN - 1) return 0;
        token->value[len++] = **input;
        (*input)++;
    }
    
    if (len > 0) {
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    *input = start;
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    size_t len = 0;
    const char *start = *input;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[len++] = **input;
        (*input)++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            if (len >= MAX_TOKEN_LEN - 1) break;
            token->value[len++] = **input;
            (*input)++;
        }
        
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    *input = start;
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
        
        while (**input && **input != '"') {
            if (len >= MAX_TOKEN_LEN - 1) {
                *input = start;
                return 0;
            }
            token->value[len++] = **input;
            (*input)++;
        }
        
        if (**input == '"') {
            (*input)++;
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            return 1;
        }
    }
    
    *input = start;
    return 0;
}

int tokenize(const char *input) {
    token_count = 0;
    
    while (*input && token_count < MAX_TOKENS) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token = {TOKEN_ERROR, ""};
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
            tokens[token_count++] = token;
            input++;
        }
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_EOF;
        strncpy(tokens[token_count].value, "", MAX_TOKEN_LEN - 1);
        tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
    }
    return 1;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "ERROR";
    }
}

void print_tokens(void) {
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s -> '%s'\n", i, token_type_name(tokens[i].type), tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(st