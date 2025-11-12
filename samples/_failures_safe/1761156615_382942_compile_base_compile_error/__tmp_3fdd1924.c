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
    
    if (isdigit(str[0])) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_IDENTIFIER;
        }
        return TOKEN_NUMBER;
    }
    
    if (str[0] == '"') {
        if (strlen(str) < 2 || str[strlen(str)-1] != '"') return TOKEN_IDENTIFIER;
        return TOKEN_STRING;
    }
    
    if (is_operator(str[0])) {
        if (strlen(str) == 1) return TOKEN_OPERATOR;
        return TOKEN_IDENTIFIER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_IDENTIFIER;
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    *tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!*tokens) return 0;
    
    *token_count = 0;
    size_t input_len = strlen(input);
    size_t pos = 0;
    
    while (pos < input_len && *token_count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) pos++;
        if (pos >= input_len) break;
        
        size_t start = pos;
        Token token = {TOKEN_EOF, ""};
        
        if (is_operator(input[pos])) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = input[pos];
            token.value[1] = '\0';
            pos++;
        } else if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            token.value[i++] = input[pos++];
            while (pos < input_len && input[pos] != '"' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            if (pos < input_len && input[pos] == '"') {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        } else {
            while (pos < input_len && !isspace(input[pos]) && !is_operator(input[pos]) && 
                   input[pos] != '"' && (pos == start || (isalnum(input[pos]) || input[pos] == '_'))) {
                pos++;
            }
            
            size_t len = pos - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, &input[start], len);
            token.value[len] = '\0';
            token.type = classify_token(token.value);
        }
        
        if (strlen(token.value) > 0) {
            (*tokens)[*token_count] = token;
            (*token_count)++;
        }
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: