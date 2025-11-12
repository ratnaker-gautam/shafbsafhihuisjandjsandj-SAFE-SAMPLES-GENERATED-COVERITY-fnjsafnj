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
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_expression(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return 0;
    
    Token* result = malloc(MAX_TOKENS * sizeof(Token));
    if (!result) return 0;
    
    size_t count = 0;
    size_t i = 0;
    
    while (i < len && count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        
        if (isdigit(input[i])) {
            while (i < len && (isdigit(input[i]) || input[i] == '.')) i++;
            size_t token_len = i - start;
            if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            token.type = TOKEN_NUMBER;
        }
        else if (is_operator(input[i])) {
            token.value[0] = input[i];
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            i++;
        }
        else if (input[i] == '"') {
            i++;
            start = i;
            while (i < len && input[i] != '"') i++;
            if (i < len) {
                size_t token_len = i - start;
                if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
                strncpy(token.value, input + start, token_len);
                token.value[token_len] = '\0';
                token.type = TOKEN_STRING;
                i++;
            }
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            while (i < len && (isalnum(input[i]) || input[i] == '_')) i++;
            size_t token_len = i - start;
            if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            token.type = TOKEN_IDENTIFIER;
        }
        else {
            i++;
            continue;
        }
        
        if (token.type != TOKEN_EOF) {
            result[count] = token;
            count++;
        }
    }
    
    *tokens = result;
    *token_count = count;
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;