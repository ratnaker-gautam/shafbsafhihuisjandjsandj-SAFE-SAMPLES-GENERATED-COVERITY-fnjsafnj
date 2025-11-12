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
    if (capacity == 0 || capacity > 1000) return NULL;
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) return NULL;
    stack->tokens = malloc(sizeof(Token) * capacity);
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

int pop_token(TokenStack* stack, Token* token) {
    if (!stack || !token || stack->count == 0) return 0;
    stack->count--;
    *token = stack->tokens[stack->count];
    return 1;
}

Token* tokenize(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return NULL;
    
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!tokens) return NULL;
    
    size_t count = 0;
    size_t i = 0;
    
    while (i < len && count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (i < len && isdigit(input[i])) {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
        } else if (isalpha(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            while (i < len && (isalnum(input[i]) || input[i] == '_')) {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            i++;
            start = i;
            while (i < len && input[i] != '"') {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
            if (i < len && input[i] == '"') i++;
        } else {
            token.type = TOKEN_OPERATOR;
            if (i - start >= MAX_TOKEN_LEN - 1) break;
            i++;
        }
        
        size_t token_len = i - start;
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            tokens[count] = token;
            count++;
        }
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
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    size_t token_count;
    Token* tokens = tokenize(input, &token_count);
    if (!tokens) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        fprintf(stderr, "Stack creation failed\n");
        free(tokens);
        return 1;
    }
    
    printf("Tokens found: %zu\n", token_count);
    printf("Pushing tokens to stack...\n");
    
    for (size_t i = 0; i < token_count; i++) {
        if (!push_token(stack, tokens[i])) {
            fprintf(stderr, "Stack push failed\n");
            break;
        }
        printf("Pushed: ");
        print_token_type(tokens[i].type);
        printf(" '%s'\n", tokens[i