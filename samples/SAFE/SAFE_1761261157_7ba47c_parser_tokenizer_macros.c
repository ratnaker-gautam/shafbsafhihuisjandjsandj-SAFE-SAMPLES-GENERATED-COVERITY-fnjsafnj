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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

TokenType classify_token(const char* token) {
    if (isdigit(token[0])) {
        for (size_t i = 0; token[i]; i++) {
            if (!isdigit(token[i]) && token[i] != '.') return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(token[0]) || token[0] == '_') {
        for (size_t i = 0; token[i]; i++) {
            if (!isalnum(token[i]) && token[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator(token[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (token[0] == '"' && token[strlen(token)-1] == '"') {
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (*pos && strchr(DELIMITERS, *pos)) pos++;
        if (!*pos) break;
        
        size_t len = 0;
        if (*pos == '"') {
            buffer[len++] = *pos++;
            while (*pos && *pos != '"' && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *pos++;
            }
            if (*pos == '"') {
                buffer[len++] = *pos++;
            }
        } else if (is_operator(*pos)) {
            buffer[len++] = *pos++;
            if (is_operator(*pos) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *pos++;
            }
        } else {
            while (*pos && !strchr(DELIMITERS, *pos) && !is_operator(*pos) && 
                   *pos != '"' && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *pos++;
            }
        }
        
        if (len > 0) {
            buffer[len] = '\0';
            if (token_count < max_tokens) {
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
            }
        }
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
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
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%-15s' Type: ", i + 1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}