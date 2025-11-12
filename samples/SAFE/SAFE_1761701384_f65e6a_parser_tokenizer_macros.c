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
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<' || c == '>';
}

int tokenize_string(const char *input, Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        Token *current = &tokens[token_count];
        memset(current->value, 0, MAX_TOKEN_LEN);
        
        if (isdigit(*ptr)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (*ptr && isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        } else if (is_operator(*ptr)) {
            current->type = TOKEN_OPERATOR;
            current->value[0] = *ptr++;
            current->value[1] = '\0';
            token_count++;
        } else if (isalpha(*ptr) || *ptr == '_') {
            current->type = TOKEN_WORD;
            int i = 0;
            while (*ptr && (isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        } else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *ptr++;
            current->value[1] = '\0';
            token_count++;
        }
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count = tokenize_string(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("Found %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: [%s] Type: ", i + 1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}