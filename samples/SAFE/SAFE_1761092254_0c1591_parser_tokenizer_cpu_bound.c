//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 100

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        }
        else if (*ptr == '"') {
            int i = 0;
            ptr++;
            while (*ptr != '"' && *ptr && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            int i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
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
    }
}

int main() {
    char input[4096];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
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