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

Token* tokenize(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!tokens) return NULL;
    
    size_t count = 0;
    size_t i = 0;
    
    while (input[i] != '\0' && count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        if (isdigit(input[i])) {
            size_t j = 0;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                tokens[count].value[j++] = input[i++];
            }
            tokens[count].value[j] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
        } else if (isalpha(input[i])) {
            size_t j = 0;
            while (isalnum(input[i]) && j < MAX_TOKEN_LEN - 1) {
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
        } else {
            i++;
        }
    }
    
    if (count < MAX_TOKENS) {
        tokens[count].type = TOKEN_EOF;
        strcpy(tokens[count].value, "");
        count++;
    }
    
    *token_count = count;
    return tokens;
}

void print_tokens(const Token* tokens, size_t count) {
    if (!tokens) return;
    for (size_t i = 0; i < count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %zu: Type=%s, Value='%s'\n", i, type_str, tokens[i].value);
    }
}

int main() {
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
    
    size_t token_count;
    Token* tokens = tokenize(input, &token_count);
    if (!tokens) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);
    if (!stack) {
        fprintf(stderr, "Stack creation failed\n");
        free(tokens);
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens(tokens, token_count);
    
    printf("\nPushing tokens to stack...\n");
    for (size_t i = 0; i < token_count; i++) {
        if (!push_token(stack, tokens[i])) {