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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        int buf_len = 0;
        if (is_operator(*ptr)) {
            buffer[buf_len++] = *ptr++;
        } else if (*ptr == '"') {
            buffer[buf_len++] = *ptr++;
            while (*ptr && *ptr != '"' && buf_len < MAX_TOKEN_LEN - 1) {
                buffer[buf_len++] = *ptr++;
            }
            if (*ptr == '"') {
                buffer[buf_len++] = *ptr++;
            }
        } else {
            while (*ptr && !strchr(DELIMITERS, *ptr) && !is_operator(*ptr) && 
                   buf_len < MAX_TOKEN_LEN - 1) {
                buffer[buf_len++] = *ptr++;
            }
        }
        
        if (buf_len > 0) {
            buffer[buf_len] = '\0';
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
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}