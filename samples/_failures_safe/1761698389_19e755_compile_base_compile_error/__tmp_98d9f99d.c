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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || strlen(str) == 0) return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return 0;
    
    Token* heap_tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!heap_tokens) return 0;
    
    size_t count = 0;
    size_t i = 0;
    
    while (i < len && count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token;
        size_t start = i;
        
        if (isdigit(input[i])) {
            while (i < len && (isdigit(input[i]) || input[i] == '.')) i++;
        } else if (isalpha(input[i])) {
            while (i < len && (isalnum(input[i]) || input[i] == '_')) i++;
        } else if (input[i] == '"') {
            i++;
            while (i < len && input[i] != '"') i++;
            if (i < len) i++;
        } else if (is_operator(input[i])) {
            i++;
        } else {
            i++;
            continue;
        }
        
        size_t token_len = i - start;
        if (token_len == 0 || token_len >= MAX_TOKEN_LEN) continue;
        
        strncpy(token.value, input + start, token_len);
        token.value[token_len] = '\0';
        token.type = classify_token(token.value);
        
        heap_tokens[count] = token;
        count++;
    }
    
    *tokens = heap_tokens;
    *token_count = count;
    return 1;
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

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    Token* tokens = NULL;
    size_t token_count = 0;
    
    if (!tokenize_string(input, &tokens, &token_count)) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(token_count);
    if (!stack) {
        printf("Stack creation failed\n");
        free(tokens);
        return 1;
    }
    
    printf("Tokens (%zu):\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("  %s -> ",