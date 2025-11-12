//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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

int pop_token(TokenStack* stack, Token* token) {
    if (!stack || !token || stack->count == 0) return 0;
    stack->count--;
    *token = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

Token* tokenize_expression(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!tokens) return NULL;
    
    size_t count = 0;
    size_t i = 0;
    
    while (input[i] != '\0' && count < MAX_TOKENS) {
        while (isspace((unsigned char)input[i])) i++;
        
        if (input[i] == '\0') break;
        
        if (isdigit((unsigned char)input[i])) {
            size_t j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                tokens[count].value[j++] = input[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
        } else if (isalpha((unsigned char)input[i])) {
            size_t j = 0;
            while (isalnum((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                tokens[count].value[j++] = input[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_IDENTIFIER;
            count++;
        } else if (is_operator(input[i])) {
            tokens[count].value[0] = input[i++];
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            count++;
        } else if (input[i] == '"') {
            size_t j = 0;
            tokens[count].value[j++] = input[i++];
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 2) {
                tokens[count].value[j++] = input[i++];
            }
            if (input[i] == '"') {
                tokens[count].value[j++] = input[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_STRING;
            count++;
        } else {
            i++;
        }
    }
    
    if (count < MAX_TOKENS) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "EOF");
        count++;
    }
    
    *token_count = count;
    return tokens;
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
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    size_t token_count = 0;
    Token* tokens = tokenize_expression(input, &token_count);
    
    if (!tokens) {
        fprintf(stderr, "Token