//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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

int parse_number(const char **input, Token *token) {
    int i = 0;
    while (**input && (isdigit(**input) || **input == '.')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    (*input)++;
    
    int i = 0;
    while (**input && **input != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    
    if (**input != '"') return 0;
    (*input)++;
    
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int tokenize(const char *input, Token *tokens, int *token_count) {
    *token_count = 0;
    
    while (*input && *token_count < MAX_TOKENS) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        int success = 0;
        
        if (isdigit(*input)) {
            success = parse_number(&input, &token);
        } else if (isalpha(*input) || *input == '_') {
            success = parse_identifier(&input, &token);
        } else if (is_operator_char(*input)) {
            success = parse_operator(&input, &token);
        } else if (*input == '"') {
            success = parse_string(&input, &token);
        } else {
            break;
        }
        
        if (!success) break;
        
        tokens[(*token_count)++] = token;
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "");
        (*token_count)++;
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
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
    
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    if (!tokenize(input, tokens, &token_count)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count - 1);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}