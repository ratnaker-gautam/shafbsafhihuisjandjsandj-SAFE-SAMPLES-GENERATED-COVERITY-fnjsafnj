//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

TokenStack* create_token_stack(size_t capacity) {
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

void destroy_token_stack(TokenStack* stack) {
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

Token pop_token(TokenStack* stack) {
    Token empty = {TOKEN_EOF, ""};
    if (!stack || stack->count == 0) return empty;
    stack->count--;
    return stack->tokens[stack->count];
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

Token* tokenize_expression(const char* input, size_t* token_count) {
    if (!input || !token_count) return NULL;
    *token_count = 0;
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!tokens) return NULL;
    
    const char* ptr = input;
    while (*ptr && *token_count < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        Token token = {TOKEN_EOF, ""};
        
        if (isdigit(*ptr)) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        } else if (isalpha(*ptr)) {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        } else if (is_operator(*ptr)) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = *ptr++;
            token.value[1] = '\0';
        } else if (*ptr == '"') {
            token.type = TOKEN_STRING;
            ptr++;
            size_t i = 0;
            while (*ptr != '"' && *ptr && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
            if (*ptr == '"') ptr++;
        } else {
            ptr++;
            continue;
        }
        
        tokens[*token_count] = token;
        (*token_count)++;
    }
    
    return tokens;
}

void evaluate_expression(Token* tokens, size_t token_count) {
    if (!tokens || token_count == 0) return;
    
    TokenStack* stack = create_token_stack(MAX_TOKENS);
    if (!stack) return;
    
    for (size_t i = 0; i < token_count; i++) {
        if (!push_token(stack, tokens[i])) {
            break;
        }
    }
    
    printf("Evaluated tokens in reverse:\n");
    while (stack->count > 0) {
        Token token = pop_token(stack);
        switch (token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", token.value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", token.value);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s\n", token.value);
                break;
            case TOKEN_STRING:
                printf("String: \"%s\"\n", token.value);
                break;
            default:
                break;
        }
    }
    
    destroy_token_stack(stack);
}

int main() {
    char input[1024];
    printf("Enter an expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    size_t token_count;
    Token* tokens = tokenize_expression(input, &token_count);
    
    if (!tokens) {
        printf("Memory allocation failed\n