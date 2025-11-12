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

void process_tokens(Token* tokens, size_t token_count) {
    if (!tokens || token_count == 0) return;
    
    TokenStack* stack = create_stack(20);
    if (!stack) return;
    
    for (size_t i = 0; i < token_count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (tokens[i].type == TOKEN_NUMBER || tokens[i].type == TOKEN_IDENTIFIER) {
            if (!push_token(stack, tokens[i])) {
                printf("Stack overflow for token: %s\n", tokens[i].value);
                break;
            }
        } else if (tokens[i].type == TOKEN_OPERATOR) {
            Token op1, op2;
            if (pop_token(stack, &op1) && pop_token(stack, &op2)) {
                printf("Operation: %s %s %s\n", op2.value, tokens[i].value, op1.value);
                Token result;
                result.type = TOKEN_NUMBER;
                snprintf(result.value, MAX_TOKEN_LEN, "result_%zu", i);
                if (!push_token(stack, result)) {
                    printf("Cannot push result\n");
                    break;
                }
            } else {
                printf("Not enough operands for operator: %s\n", tokens[i].value);
                break;
            }
        }
    }
    
    Token final;
    if (pop_token(stack, &final)) {
        printf("Final result: %s\n", final.value);
    }
    
    destroy_stack(stack);
}

int main() {
    char input[256];
    printf("Enter expression: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n