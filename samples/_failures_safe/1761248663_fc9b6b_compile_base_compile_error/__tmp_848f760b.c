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
    TOKEN_EOF
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
    char *end = token->value;
    
    while (*p && isdigit(*p)) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    *end = '\0';
    token->type = TOKEN_NUMBER;
    *input = p;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (!isalpha(*p) && *p != '_') {
        return 0;
    }
    
    while (*p && (isalnum(*p) || *p == '_')) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    *end = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = p;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (!is_operator_char(*p)) {
        return 0;
    }
    
    *end++ = *p++;
    if (is_operator_char(*p) && (*(p-1) == '<' || *(p-1) == '>' || *(p-1) == '!' || *(p-1) == '=')) {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    *end = '\0';
    token->type = TOKEN_OPERATOR;
    *input = p;
    return 1;
}

int parse_string(const char **input, Token *token) {
    const char *p = *input;
    char *end = token->value;
    
    if (*p != '"') {
        return 0;
    }
    
    p++;
    while (*p && *p != '"') {
        if ((size_t)(end - token->value) >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *end++ = *p++;
    }
    
    if (*p != '"') {
        return 0;
    }
    
    p++;
    *end = '\0';
    token->type = TOKEN_STRING;
    *input = p;
    return 1;
}

int tokenize(const char *input, Token *tokens, size_t max_tokens) {
    const char *p = input;
    size_t token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) {
            p++;
        }
        
        if (!*p) {
            break;
        }
        
        Token *current = tokens + token_count;
        
        if (parse_number(&p, current) ||
            parse_string(&p, current) ||
            parse_identifier(&p, current) ||
            parse_operator(&p, current)) {
            token_count++;
        } else {
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return (int)token_count;
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
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        fprintf(stderr, "Error: Invalid token encountered\n");
        return 1;
    }
    
    printf("Tokens found:\n");
    for (int i = 0; i < token_count; i++) {