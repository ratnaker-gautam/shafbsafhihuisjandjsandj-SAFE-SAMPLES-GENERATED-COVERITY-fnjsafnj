//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n"

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, Token* tokens, int* token_count) {
    const char* p = input;
    *token_count = 0;
    
    while (*p && *token_count < MAX_TOKENS) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;
        
        Token* token = &tokens[(*token_count)++];
        
        if (isdigit(*p)) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (*p && (isdigit(*p) || *p == '.') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
        }
        else if (isalpha(*p) || *p == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while (*p && (isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
        }
        else if (*p == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            p++;
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
            if (*p == '"') p++;
        }
        else if (is_operator(*p)) {
            token->type = TOKEN_OPERATOR;
            int i = 0;
            while (*p && is_operator(*p) && i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = *p++;
            }
            token->value[i] = '\0';
        }
        else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = *p++;
            token->value[1] = '\0';
        }
    }
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count;
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    tokenize(input, tokens, &token_count);
    
    printf("\nTokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}