//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenStack;

TokenStack* create_stack(size_t capacity) {
    if (capacity == 0 || capacity > MAX_TOKENS) return NULL;
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) return NULL;
    stack->tokens = malloc(capacity * sizeof(Token));
    if (!stack->tokens) {
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->count = 0;
    return stack;
}

void destroy_stack(TokenStack* stack) {
    if (stack) {
        free(stack->tokens);
        free(stack);
    }
}

int push_token(TokenStack* stack, Token token) {
    if (!stack || stack->count >= stack->capacity) return 0;
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    stack->tokens[stack->count] = token;
    stack->count++;
    return 1;
}

int pop_token(TokenStack* stack, Token* result) {
    if (!stack || !result || stack->count == 0) return 0;
    stack->count--;
    *result = stack->tokens[stack->count];
    return 1;
}

Token* tokenize_expression(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!tokens) return NULL;
    
    size_t count = 0;
    const char* ptr = input;
    
    while (*ptr && count < MAX_TOKENS) {
        while (isspace((unsigned char)*ptr)) ptr++;
        if (!*ptr) break;
        
        if (isdigit((unsigned char)*ptr)) {
            size_t len = 0;
            while (isdigit((unsigned char)*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *ptr++;
            }
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
        } else if (isalpha((unsigned char)*ptr)) {
            size_t len = 0;
            while (isalnum((unsigned char)*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *ptr++;
            }
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_IDENTIFIER;
            count++;
        } else if (*ptr == '"') {
            ptr++;
            size_t len = 0;
            while (*ptr != '"' && *ptr && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_STRING;
            count++;
        } else {
            tokens[count].value[0] = *ptr;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            count++;
            ptr++;
        }
    }
    
    if (count < MAX_TOKENS) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "");
        count++;
    }
    
    *token_count = count;
    return tokens;
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

int main(void) {
    char input[256];
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    size_t token_count;
    Token* tokens = tokenize_expression(input, &token_count);
    if (!tokens) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        printf("Stack creation failed\n");
        free(tokens);
        return 1;
    }
    
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("Type: ");
        print_token_type(tokens[i].type);
        printf(", Value: '%s'\n", tokens[i].value);
        
        if (push_token(stack, tokens[i]) == 0) {