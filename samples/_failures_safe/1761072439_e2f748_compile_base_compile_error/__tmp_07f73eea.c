//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize_expression(const char* input, Token* tokens, size_t max_tokens) {
    if (!input || !tokens || max_tokens == 0) return 0;
    
    size_t token_count = 0;
    size_t i = 0;
    
    while (input[i] != '\0' && token_count < max_tokens) {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        Token token;
        size_t j = 0;
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (isalpha(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            i++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            if (input[i] == '"') i++;
        } else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            token.value[j++] = input[i++];
        } else {
            i++;
            continue;
        }
        
        token.value[j] = '\0';
        tokens[token_count++] = token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
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
        default: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[256];
    printf("Enter an expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize_expression(input, tokens, MAX_TOKENS);
    
    if (token_count <= 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        printf("Failed to create token stack\n");
        return 1;
    }
    
    printf("\nTokens found (%d):\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i + 1);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i