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
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (*pos && strchr(DELIMITERS, *pos)) pos++;
        if (!*pos) break;
        
        size_t len = 0;
        if (is_operator_char(*pos)) {
            buffer[len++] = *pos++;
            if (is_operator_char(*pos)) buffer[len++] = *pos++;
        } else if (*pos == '"') {
            buffer[len++] = *pos++;
            while (*pos && *pos != '"' && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *pos++;
            }
            if (*pos == '"') buffer[len++] = *pos++;
        } else {
            while (*pos && !strchr(DELIMITERS, *pos) && 
                   !is_operator_char(*pos) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *pos++;
            }
        }
        
        if (len > 0) {
            buffer[len] = '\0';
            tokens[token_count].type = classify_token(buffer);
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
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
        default: printf("UNKNOWN"); break;
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}