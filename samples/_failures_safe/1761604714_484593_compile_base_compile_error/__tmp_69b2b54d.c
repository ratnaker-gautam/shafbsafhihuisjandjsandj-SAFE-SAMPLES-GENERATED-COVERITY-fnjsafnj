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
    size_t count;
    size_t capacity;
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
    stack->count = 0;
    stack->capacity = capacity;
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
    
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return NULL;
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!tokens) return NULL;
    
    size_t count = 0;
    size_t i = 0;
    
    while (i < len && count < MAX_TOKENS) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }
        
        if (isdigit((unsigned char)input[i])) {
            size_t start = i;
            while (i < len && isdigit((unsigned char)input[i])) i++;
            if (i - start >= MAX_TOKEN_LEN) {
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count].value, &input[start], i - start);
            tokens[count].value[i - start] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
            continue;
        }
        
        if (isalpha((unsigned char)input[i])) {
            size_t start = i;
            while (i < len && isalnum((unsigned char)input[i])) i++;
            if (i - start >= MAX_TOKEN_LEN) {
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count].value, &input[start], i - start);
            tokens[count].value[i - start] = '\0';
            tokens[count].type = TOKEN_IDENTIFIER;
            count++;
            continue;
        }
        
        if (input[i] == '"') {
            size_t start = i;
            i++;
            while (i < len && input[i] != '"') i++;
            if (i >= len) {
                free(tokens);
                return NULL;
            }
            i++;
            if (i - start - 2 >= MAX_TOKEN_LEN) {
                free(tokens);
                return NULL;
            }
            strncpy(tokens[count].value, &input[start + 1], i - start - 2);
            tokens[count].value[i - start - 2] = '\0';
            tokens[count].type = TOKEN_STRING;
            count++;
            continue;
        }
        
        if (strchr("+-*/=<>!&|", input[i])) {
            if (i + 1 < len && strchr("=<>|&", input[i + 1])) {
                if (2 >= MAX_TOKEN_LEN) {
                    free(tokens);
                    return NULL;
                }
                tokens[count].value[0] = input[i];
                tokens[count].value[1] = input[i + 1];
                tokens[count].value[2] = '\0';
                i += 2;
            } else {
                if (1 >= MAX_TOKEN_LEN) {
                    free(tokens);
                    return NULL;
                }
                tokens[count].value[0] = input[i];
                tokens[count].value[1] = '\0';
                i++;
            }
            tokens[count].type = TOKEN_OPERATOR;
            count++;
            continue;
        }
        
        i++;
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
        case TOKEN_OPERATOR: