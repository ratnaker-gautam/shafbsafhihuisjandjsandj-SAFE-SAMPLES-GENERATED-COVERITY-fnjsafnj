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
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

TokenType classify_token(const char* token) {
    if (isdigit(token[0])) {
        return TOKEN_NUMBER;
    }
    if (is_operator(token[0]) && token[1] == '\0') {
        return TOKEN_OPERATOR;
    }
    if (isalpha(token[0])) {
        return TOKEN_IDENTIFIER;
    }
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        int buf_index = 0;
        while (*ptr && !strchr(DELIMITERS, *ptr) && buf_index < MAX_TOKEN_LEN - 1) {
            buffer[buf_index++] = *ptr++;
        }
        buffer[buf_index] = '\0';
        
        if (buf_index > 0) {
            tokens[token_count].type = classify_token(buffer);
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
    }
    
    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    for (int i = 0; i < count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s ('%s')\n", i + 1, type_str, tokens[i].value);
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found in input\n");
        return 0;
    }
    
    if (token_count >= MAX_TOKENS) {
        printf("Warning: Input contains more tokens than maximum (%d)\n", MAX_TOKENS);
    }
    
    printf("\nTokenized %d tokens:\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}