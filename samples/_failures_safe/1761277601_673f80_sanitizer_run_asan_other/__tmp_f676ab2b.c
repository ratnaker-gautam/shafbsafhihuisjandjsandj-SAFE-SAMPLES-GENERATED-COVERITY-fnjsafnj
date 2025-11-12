//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (strlen(str) == 0) return TOKEN_ERROR;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isdigit(str[i])) return TOKEN_ERROR;
        }
        return TOKEN_NUMBER;
    }
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_ERROR;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (strlen(str) == 1 && is_operator(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    return TOKEN_ERROR;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    char buffer[MAX_TOKEN_LEN];
    char* copy = strdup(input);
    if (!copy) return -1;
    
    int token_count = 0;
    char* token_str = strtok(copy, DELIMITERS);
    
    while (token_str && token_count < max_tokens) {
        if (strlen(token_str) >= MAX_TOKEN_LEN) {
            free(copy);
            return -1;
        }
        
        strncpy(buffer, token_str, MAX_TOKEN_LEN - 1);
        buffer[MAX_TOKEN_LEN - 1] = '\0';
        
        TokenType type = classify_token(buffer);
        
        if (type != TOKEN_ERROR) {
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].type = type;
            token_count++;
        }
        
        token_str = strtok(NULL, DELIMITERS);
    }
    
    free(copy);
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
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
        default: printf("ERROR"); break;
    }
}

int main(void) {
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
    int count = tokenize(input, tokens, MAX_TOKENS);
    
    if (count <= 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}