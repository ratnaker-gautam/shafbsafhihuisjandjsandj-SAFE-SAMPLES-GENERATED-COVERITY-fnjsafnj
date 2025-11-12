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

TokenType classify_token(const char* token) {
    if (isdigit(token[0])) {
        for (int i = 0; token[i]; i++) {
            if (!isdigit(token[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(token[0]) || token[0] == '_') {
        for (int i = 0; token[i]; i++) {
            if (!isalnum(token[i]) && token[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (token[0] == '"' && token[strlen(token)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (strlen(token) == 1 && is_operator(token[0])) {
        return TOKEN_OPERATOR;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens) {
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        int buf_pos = 0;
        if (*ptr == '"') {
            buffer[buf_pos++] = *ptr++;
            while (*ptr && *ptr != '"' && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = *ptr++;
            }
            if (*ptr == '"') {
                buffer[buf_pos++] = *ptr++;
            }
        } else if (is_operator(*ptr)) {
            buffer[buf_pos++] = *ptr++;
        } else {
            while (*ptr && !strchr(DELIMITERS, *ptr) && !is_operator(*ptr) && 
                   buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = *ptr++;
            }
        }
        
        if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            tokens[token_count].type = classify_token(buffer);
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "EOF");
    return token_count + 1;
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

int main() {
    char input[1024];
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
    
    Token tokens[MAX_TOKENS];
    int total_tokens = tokenize(input, tokens);
    
    printf("Tokens found: %d\n", total_tokens - 1);
    printf("Token list:\n");
    
    for (int i = 0; i < total_tokens; i++) {
        printf("  ");
        print_token_type(tokens[i].type);
        printf(": '%s'\n", tokens[i].value);
    }
    
    return 0;
}