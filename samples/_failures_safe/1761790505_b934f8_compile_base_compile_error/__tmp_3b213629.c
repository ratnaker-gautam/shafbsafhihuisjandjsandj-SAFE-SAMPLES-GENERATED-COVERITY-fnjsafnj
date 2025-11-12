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
    
    *tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!*tokens) return 0;
    
    size_t count = 0;
    size_t i = 0;
    
    while (i < len && count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        size_t j = 0;
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            i++;
            while (i < len && input[i] != '"' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i];
                i++;
            }
            token.value[j] = '\0';
            if (i < len && input[i] == '"') i++;
        } else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            if (j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i];
                token.value[j] = '\0';
            }
            i++;
        } else if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (i < len && (isdigit(input[i]) || input[i] == '.') && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i];
                i++;
            }
            token.value[j] = '\0';
        } else if (isalpha(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            while (i < len && (isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i];
                i++;
            }
            token.value[j] = '\0';
        } else {
            i++;
            continue;
        }
        
        if (strlen(token.value) < MAX_TOKEN_LEN) {
            (*tokens)[count] = token;
            count++;
        }
    }
    
    *token_count = count;
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
        printf("Error reading input\n");
        return 1