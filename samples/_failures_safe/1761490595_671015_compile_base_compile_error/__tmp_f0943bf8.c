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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || strlen(str) == 0) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    
    if (str[0] == '"' && strlen(str) > 1 && str[strlen(str)-1] == '"') return TOKEN_STRING;
    
    if (is_operator(str[0]) && strlen(str) == 1) return TOKEN_OPERATOR;
    
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_input(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t len = strlen(input);
    if (len == 0) return 0;
    
    *tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!*tokens) return 0;
    
    *token_count = 0;
    size_t pos = 0;
    
    while (pos < len && *token_count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = pos;
        
        if (input[pos] == '"') {
            pos++;
            while (pos < len && input[pos] != '"') {
                if (pos - start >= MAX_TOKEN_LEN - 1) break;
                pos++;
            }
            if (pos < len && input[pos] == '"') pos++;
        } else if (is_operator(input[pos])) {
            pos++;
        } else if (isdigit(input[pos])) {
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                if (pos - start >= MAX_TOKEN_LEN - 1) break;
                pos++;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (pos - start >= MAX_TOKEN_LEN - 1) break;
                pos++;
            }
        } else {
            pos++;
            continue;
        }
        
        size_t token_len = pos - start;
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            token.type = classify_token(token.value);
            
            if (token.type != TOKEN_EOF) {
                (*tokens)[*token_count] = token;
                (*token_count)++;
            }
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
        default: printf("UNKNOWN"); break;
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