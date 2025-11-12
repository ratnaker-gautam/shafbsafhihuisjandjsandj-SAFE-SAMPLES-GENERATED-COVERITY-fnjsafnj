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
    if (str[0] == '"' && str[strlen(str)-1] == '"') return TOKEN_STRING;
    if (is_operator(str[0]) && strlen(str) == 1) return TOKEN_OPERATOR;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    *tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!*tokens) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    size_t token_pos = 0;
    int in_string = 0;
    
    for (size_t i = 0; input[i] != '\0' && token_pos < MAX_TOKENS; i++) {
        char c = input[i];
        
        if (in_string) {
            if (c == '"') {
                if (buf_pos < MAX_TOKEN_LEN - 2) {
                    buffer[buf_pos] = '"';
                    buffer[buf_pos + 1] = '\0';
                } else {
                    buffer[MAX_TOKEN_LEN - 1] = '\0';
                }
                (*tokens)[token_pos].type = TOKEN_STRING;
                strncpy((*tokens)[token_pos].value, buffer, MAX_TOKEN_LEN-1);
                (*tokens)[token_pos].value[MAX_TOKEN_LEN-1] = '\0';
                token_pos++;
                buf_pos = 0;
                in_string = 0;
            } else if (buf_pos < MAX_TOKEN_LEN - 2) {
                buffer[buf_pos++] = c;
            }
            continue;
        }
        
        if (isspace(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                (*tokens)[token_pos].type = classify_token(buffer);
                strncpy((*tokens)[token_pos].value, buffer, MAX_TOKEN_LEN-1);
                (*tokens)[token_pos].value[MAX_TOKEN_LEN-1] = '\0';
                token_pos++;
                buf_pos = 0;
            }
            continue;
        }
        
        if (c == '"') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                (*tokens)[token_pos].type = classify_token(buffer);
                strncpy((*tokens)[token_pos].value, buffer, MAX_TOKEN_LEN-1);
                (*tokens)[token_pos].value[MAX_TOKEN_LEN-1] = '\0';
                token_pos++;
                buf_pos = 0;
            }
            if (buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = '"';
            }
            in_string = 1;
            continue;
        }
        
        if (is_operator(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                (*tokens)[token_pos].type = classify_token(buffer);
                strncpy((*tokens)[token_pos].value, buffer, MAX_TOKEN_LEN-1);
                (*tokens)[token_pos].value[MAX_TOKEN_LEN-1] = '\0';
                token_pos++;
                buf_pos = 0;
            }
            buffer[