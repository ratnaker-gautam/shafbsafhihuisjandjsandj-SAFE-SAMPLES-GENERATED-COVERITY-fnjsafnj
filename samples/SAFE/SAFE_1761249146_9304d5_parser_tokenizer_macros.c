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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        return TOKEN_NUMBER;
    } else if (is_operator(str[0])) {
        return TOKEN_OPERATOR;
    } else if (isalpha(str[0])) {
        return TOKEN_WORD;
    }
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    char buffer[MAX_TOKEN_LEN];
    char* token_str;
    char input_copy[1024];
    int token_count = 0;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return -1;
    }
    
    if (strlen(input) >= sizeof(input_copy)) {
        return -1;
    }
    
    strncpy(input_copy, input, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';
    
    token_str = strtok(input_copy, DELIMITERS);
    
    while (token_str != NULL && token_count < max_tokens) {
        if (strlen(token_str) < MAX_TOKEN_LEN) {
            tokens[token_count].type = classify_token(token_str);
            strncpy(tokens[token_count].value, token_str, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        token_str = strtok(NULL, DELIMITERS);
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER:
            printf("NUMBER");
            break;
        case TOKEN_WORD:
            printf("WORD");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    int token_count;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[sizeof(input) - 1] = '\0';
    
    token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Error during tokenization\n");
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