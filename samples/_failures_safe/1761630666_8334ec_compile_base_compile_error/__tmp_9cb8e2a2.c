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

int pop_token(TokenStack* stack, Token* result) {
    if (!stack || !result || stack->count == 0) return 0;
    stack->count--;
    *result = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize_expression(const char* input, TokenStack* stack) {
    if (!input || !stack) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return 0;
    
    size_t i = 0;
    while (i < len) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token;
        memset(&token, 0, sizeof(token));
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && is_operator(input[i])) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = 0;
            token.value[j++] = input[i++];
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
            }
            if (i < len && input[i] == '"') {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else {
            i++;
            continue;
        }
        
        if (!push_token(stack, token)) {
            return 0;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.value, "EOF");
    return push_token(stack, eof_token);
}

void print_tokens_reverse(TokenStack* stack) {
    if (!stack) return;
    
    Token token;
    TokenStack* temp_stack = create_stack(stack->capacity);
    if (!temp_stack) return;
    
    while (pop_token(stack, &token)) {
        if (!push_token(temp_stack, token)) break;
    }
    
    printf("Tokens in reverse order:\n");
    while (pop_token(temp_stack, &token)) {
        const char* type_str = "UNKNOWN";
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Type: %-12s Value: %s\n", type_str, token.value);
    }
    
    destroy_stack(temp_stack);
}

int main() {
    char input[1024];
    printf("Enter an expression to tokenize: ");