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

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    *tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!*tokens) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    *token_count = 0;
    
    while (pos < len && *token_count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token;
        memset(&token, 0, sizeof(Token));
        
        if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else if (is_operator(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && is_operator(input[pos])) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            pos++;
            while (pos < len && i < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
            if (pos < len && input[pos] == '"') pos++;
        }
        else {
            pos++;
            continue;
        }
        
        if (strlen(token.value) > 0) {
            (*tokens)[*token_count] = token;
            (*token_count)++;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    (*tokens)[*token_count] = eof_token;
    (*token_count)++;
    
    return 1;
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
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");