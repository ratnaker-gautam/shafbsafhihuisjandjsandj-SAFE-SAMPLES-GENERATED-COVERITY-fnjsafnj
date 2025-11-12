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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
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
    
    *tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!*tokens) return 0;
    
    size_t count = 0;
    const char* ptr = input;
    
    while (*ptr && count < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        size_t len = 0;
        Token token;
        token.type = classify_token(ptr);
        
        switch (token.type) {
            case TOKEN_NUMBER:
                while (isdigit(ptr[len])) len++;
                break;
            case TOKEN_IDENTIFIER:
                while (isalnum(ptr[len]) || ptr[len] == '_') len++;
                break;
            case TOKEN_OPERATOR:
                len = 1;
                break;
            case TOKEN_STRING:
                len = 1;
                while (ptr[len] && ptr[len] != '"') len++;
                if (ptr[len] == '"') len++;
                break;
            default:
                ptr++;
                continue;
        }
        
        if (len >= MAX_TOKEN_LEN) {
            free(*tokens);
            return 0;
        }
        
        strncpy(token.value, ptr, len);
        token.value[len] = '\0';
        
        (*tokens)[count] = token;
        count++;
        ptr += len;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    if (count < MAX_TOKENS) {
        (*tokens)[count] = eof_token;
        count++;
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
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Token* tokens = NULL;
    size_t token_count = 0;
    
    if (!tokenize_string(input, &tokens, &token_count)) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        printf("Stack creation failed\n");
        free(tokens);
        return 1;
    }
    
    printf("Tokens:\n