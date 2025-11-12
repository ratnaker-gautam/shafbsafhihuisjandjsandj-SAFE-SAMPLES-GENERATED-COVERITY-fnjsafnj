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
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t capacity = 20;
    *tokens = malloc(capacity * sizeof(Token));
    if (!*tokens) return 0;
    
    *token_count = 0;
    size_t i = 0;
    
    while (input[i] != '\0') {
        while (isspace((unsigned char)input[i])) i++;
        if (input[i] == '\0') break;
        
        if (*token_count >= capacity) {
            if (capacity > 1000) {
                free(*tokens);
                return 0;
            }
            capacity *= 2;
            Token* new_tokens = realloc(*tokens, capacity * sizeof(Token));
            if (!new_tokens) {
                free(*tokens);
                return 0;
            }
            *tokens = new_tokens;
        }
        
        Token token;
        size_t j = 0;
        token.type = classify_token(&input[i]);
        
        if (token.type == TOKEN_NUMBER) {
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (token.type == TOKEN_IDENTIFIER) {
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (token.type == TOKEN_OPERATOR) {
            token.value[j++] = input[i++];
            if (is_operator(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else if (token.type == TOKEN_STRING) {
            token.value[j++] = input[i++];
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            if (input[i] == '"' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
        } else {
            break;
        }
        
        token.value[j] = '\0';
        (*tokens)[(*token_count)++] = token;
    }
    
    if (*token_count < capacity) {
        Token* resized = realloc(*tokens, *token_count * sizeof(Token));
        if (resized) *tokens = resized;
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TO