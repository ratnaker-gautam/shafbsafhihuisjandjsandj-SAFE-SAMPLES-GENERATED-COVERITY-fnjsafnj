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
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, Token* tokens, size_t max_tokens) {
    if (!input || !tokens || max_tokens == 0) return 0;
    
    size_t token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace((unsigned char)*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        Token current_token;
        size_t token_len = 0;
        TokenType type = classify_token(ptr);
        
        if (type == TOKEN_NUMBER) {
            while (isdigit((unsigned char)*ptr) && token_len < MAX_TOKEN_LEN - 1) {
                current_token.value[token_len++] = *ptr++;
            }
            current_token.type = TOKEN_NUMBER;
        }
        else if (type == TOKEN_IDENTIFIER) {
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && token_len < MAX_TOKEN_LEN - 1) {
                current_token.value[token_len++] = *ptr++;
            }
            current_token.type = TOKEN_IDENTIFIER;
        }
        else if (type == TOKEN_OPERATOR) {
            current_token.value[token_len++] = *ptr++;
            current_token.type = TOKEN_OPERATOR;
        }
        else if (type == TOKEN_STRING) {
            current_token.value[token_len++] = *ptr++;
            while (*ptr != '"' && *ptr != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                current_token.value[token_len++] = *ptr++;
            }
            if (*ptr == '"') {
                current_token.value[token_len++] = *ptr++;
            }
            current_token.type = TOKEN_STRING;
        }
        else {
            break;
        }
        
        current_token.value[token_len] = '\0';
        tokens[token_count++] = current_token;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return (int)token_count;
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    Token