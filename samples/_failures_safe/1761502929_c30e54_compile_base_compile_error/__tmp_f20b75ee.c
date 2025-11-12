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
    
    *token_count = 0;
    *tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!*tokens) return 0;
    
    const char* ptr = input;
    size_t token_idx = 0;
    
    while (*ptr && token_idx < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t len = 0;
        
        if (isdigit(*ptr)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else if (is_operator(*ptr)) {
            token.type = TOKEN_OPERATOR;
            token.value[len++] = *ptr++;
        } else if (*ptr == '"') {
            token.type = TOKEN_STRING;
            token.value[len++] = *ptr++;
            while (*ptr != '"' && *ptr && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
            if (*ptr == '"') {
                token.value[len++] = *ptr++;
            }
        } else if (isalpha(*ptr) || *ptr == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else {
            ptr++;
            continue;
        }
        
        token.value[len] = '\0';
        if (len > 0) {
            (*tokens)[token_idx++] = token;
        }
    }
    
    *token_count = token_idx;
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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    Token* tokens = NULL;
    size_t token_count = 0;
    
    if (!tokenize_string(input, &tokens, &token_count)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    TokenStack* stack = create_stack(MAX_TOKENS);