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

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t capacity = 20;
    *tokens = malloc(capacity * sizeof(Token));
    if (!*tokens) return 0;
    
    *token_count = 0;
    const char* ptr = input;
    
    while (*ptr && *token_count < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        if (*token_count >= capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
            Token* new_tokens = realloc(*tokens, new_capacity * sizeof(Token));
            if (!new_tokens) {
                free(*tokens);
                return 0;
            }
            *tokens = new_tokens;
            capacity = new_capacity;
        }
        
        Token token;
        size_t len = 0;
        
        if (isdigit(*ptr)) {
            while (isdigit(ptr[len]) && len < MAX_TOKEN_LEN - 1) {
                token.value[len] = ptr[len];
                len++;
            }
            token.type = TOKEN_NUMBER;
        } else if (is_operator(*ptr)) {
            token.value[0] = *ptr;
            len = 1;
            token.type = TOKEN_OPERATOR;
        } else if (*ptr == '"') {
            token.value[0] = '"';
            len = 1;
            while (ptr[len] && ptr[len] != '"' && len < MAX_TOKEN_LEN - 2) {
                token.value[len] = ptr[len];
                len++;
            }
            if (ptr[len] == '"') {
                token.value[len] = '"';
                len++;
            }
            token.type = TOKEN_STRING;
        } else if (isalpha(*ptr) || *ptr == '_') {
            while ((isalnum(ptr[len]) || ptr[len] == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len] = ptr[len];
                len++;
            }
            token.type = TOKEN_IDENTIFIER;
        } else {
            ptr++;
            continue;
        }
        
        token.value[len] = '\0';
        (*tokens)[*token_count] = token;
        (*token_count)++;
        ptr += len;
    }
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input),