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

int pop_token(TokenStack* stack, Token* token) {
    if (!stack || !token || stack->count == 0) return 0;
    stack->count--;
    *token = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || strlen(str) == 0) return TOKEN_EOF;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; str[i]; i++) {
            if (!isdigit(str[i])) return TOKEN_IDENTIFIER;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; str[i]; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_IDENTIFIER;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    *tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!*tokens) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    *token_count = 0;
    
    while (pos < len && *token_count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        size_t start = pos;
        Token current_token;
        memset(&current_token, 0, sizeof(Token));
        
        if (input[pos] == '"') {
            pos++;
            while (pos < len && input[pos] != '"') {
                if (pos - start < MAX_TOKEN_LEN - 1) {
                    current_token.value[pos - start - 1] = input[pos];
                }
                pos++;
            }
            if (pos < len && input[pos] == '"') pos++;
            current_token.type = TOKEN_STRING;
        } else if (is_operator(input[pos])) {
            current_token.value[0] = input[pos];
            current_token.value[1] = '\0';
            current_token.type = TOKEN_OPERATOR;
            pos++;
        } else {
            while (pos < len && !isspace(input[pos]) && !is_operator(input[pos]) && input[pos] != '"') {
                if (pos - start < MAX_TOKEN_LEN - 1) {
                    current_token.value[pos - start] = input[pos];
                }
                pos++;
            }
            current_token.type = classify_token(current_token.value);
        }
        
        if (strlen(current_token.value) > 0) {
            (*tokens)[*token_count] = current_token;
            (*token_count)++;
        }
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
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if