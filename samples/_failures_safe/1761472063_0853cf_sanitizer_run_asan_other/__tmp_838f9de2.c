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
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* token) {
    if (isdigit(token[0])) return TOKEN_NUMBER;
    if (isalpha(token[0]) || token[0] == '_') return TOKEN_IDENTIFIER;
    if (token[0] == '"' && token[strlen(token)-1] == '"') return TOKEN_STRING;
    if (is_operator(token[0])) return TOKEN_OPERATOR;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    char buffer[MAX_TOKEN_LEN];
    char* copy = strdup(input);
    if (!copy) return -1;
    
    char* saveptr = NULL;
    char* token_str = strtok_r(copy, DELIMITERS, &saveptr);
    int count = 0;
    
    while (token_str && count < max_tokens) {
        size_t len = strlen(token_str);
        if (len >= MAX_TOKEN_LEN) {
            free(copy);
            return -1;
        }
        
        tokens[count].type = classify_token(token_str);
        strncpy(tokens[count].value, token_str, MAX_TOKEN_LEN - 1);
        tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
        
        count++;
        token_str = strtok_r(NULL, DELIMITERS, &saveptr);
    }
    
    free(copy);
    return count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
        default: printf("INVALID"); break;
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
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}