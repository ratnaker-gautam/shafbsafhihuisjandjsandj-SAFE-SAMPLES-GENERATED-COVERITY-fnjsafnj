//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: parser_tokenizer
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
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (*pos && strchr(DELIMITERS, *pos)) pos++;
        if (!*pos) break;
        
        if (isdigit(*pos)) {
            int len = 0;
            while (isdigit(pos[len]) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = pos[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            pos += len;
            token_count++;
        }
        else if (isalpha(*pos) || *pos == '_') {
            int len = 0;
            while ((isalnum(pos[len]) || pos[len] == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len] = pos[len];
                len++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            pos += len;
            token_count++;
        }
        else if (*pos == '+' || *pos == '-' || *pos == '*' || *pos == '/' || *pos == '=') {
            tokens[token_count].value[0] = *pos;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            pos++;
            token_count++;
        }
        else if (*pos == '"') {
            int len = 0;
            tokens[token_count].value[len++] = *pos++;
            
            while (*pos && *pos != '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            
            if (*pos == '"' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *pos++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_token(const Token* token) {
    if (!token) return;
    
    switch (token->type) {
        case TOKEN_NUMBER:
            printf("NUMBER: %s\n", token->value);
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER: %s\n", token->value);
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR: %s\n", token->value);
            break;
        case TOKEN_STRING:
            printf("STRING: %s\n", token->value);
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int num_tokens = tokenize(input, tokens, MAX_TOKENS);
    if (num_tokens < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    for (int i = 0; i < num_tokens; i++) {
        printf("%d: ", i + 1);
        print_token(&tokens[i]);
    }
    
    return 0;
}