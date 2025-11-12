//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
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
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        int buf_index = 0;
        if (is_operator(*ptr)) {
            buffer[buf_index++] = *ptr++;
        } else if (*ptr == '"') {
            buffer[buf_index++] = *ptr++;
            while (*ptr && *ptr != '"' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            if (*ptr == '"') {
                buffer[buf_index++] = *ptr++;
            }
        } else {
            while (*ptr && !strchr(DELIMITERS, *ptr) && !is_operator(*ptr) && 
                   buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
        }
        
        if (buf_index > 0) {
            buffer[buf_index] = '\0';
            if (buf_index >= MAX_TOKEN_LEN) return -1;
            
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
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[sizeof(input) - 1] = '\0';
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        fprintf(stderr, "Tokenization error: token too long\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
    }
    
    return 0;
}