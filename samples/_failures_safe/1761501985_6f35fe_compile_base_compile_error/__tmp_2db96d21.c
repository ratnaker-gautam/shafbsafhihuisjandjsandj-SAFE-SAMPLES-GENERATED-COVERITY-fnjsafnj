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
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t capacity = 10;
    *tokens = malloc(sizeof(Token) * capacity);
    if (!*tokens) return 0;
    
    *token_count = 0;
    size_t i = 0;
    
    while (input[i] != '\0') {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        if (*token_count >= capacity) {
            if (capacity >= MAX_TOKENS) {
                free(*tokens);
                return 0;
            }
            capacity = (capacity * 2 > MAX_TOKENS) ? MAX_TOKENS : capacity * 2;
            Token* new_tokens = realloc(*tokens, sizeof(Token) * capacity);
            if (!new_tokens) {
                free(*tokens);
                return 0;
            }
            *tokens = new_tokens;
        }
        
        Token token;
        size_t start = i;
        size_t len = 0;
        
        token.type = classify_token(&input[i]);
        
        if (token.type == TOKEN_NUMBER) {
            while (isdigit(input[i])) {
                i++;
                len++;
            }
        } else if (token.type == TOKEN_IDENTIFIER) {
            while (isalnum(input[i]) || input[i] == '_') {
                i++;
                len++;
            }
        } else if (token.type == TOKEN_OPERATOR) {
            len = 1;
            i++;
        } else if (token.type == TOKEN_STRING) {
            len = 1;
            i++;
            while (input[i] != '"' && input[i] != '\0') {
                i++;
                len++;
            }
            if (input[i] == '"') {
                i++;
                len++;
            }
        } else {
            break;
        }
        
        if (len >= MAX_TOKEN_LEN) {
            free(*tokens);
            return 0;
        }
        
        strncpy(token.value, &input[start], len);
        token.value[len] = '\0';
        
        (*tokens)[*token_count] = token;
        (*token_count)++;
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    
    if (*token_count >= capacity) {
        if (capacity >= MAX_TOKENS) {
            free(*tokens);
            return 0;
        }
        capacity++;
        Token* new_tokens = realloc(*tokens, sizeof(Token) * capacity);
        if (!new_tokens) {
            free(*tokens);
            return 0;
        }
        *tokens = new_tokens;
    }
    
    (*tokens)[*token_count] = eof_token;
    (*token_count)++;
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf