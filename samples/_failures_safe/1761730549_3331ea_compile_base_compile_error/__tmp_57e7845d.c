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

typedef struct {
    Token** tokens_heap;
    size_t heap_size;
    size_t heap_capacity;
} TokenHeap;

TokenHeap* create_heap(size_t capacity) {
    if (capacity == 0 || capacity > 1000) return NULL;
    TokenHeap* heap = malloc(sizeof(TokenHeap));
    if (!heap) return NULL;
    heap->tokens_heap = malloc(capacity * sizeof(Token*));
    if (!heap->tokens_heap) {
        free(heap);
        return NULL;
    }
    heap->heap_capacity = capacity;
    heap->heap_size = 0;
    return heap;
}

void destroy_heap(TokenHeap* heap) {
    if (heap) {
        for (size_t i = 0; i < heap->heap_size; i++) {
            free(heap->tokens_heap[i]);
        }
        free(heap->tokens_heap);
        free(heap);
    }
}

int add_to_heap(TokenHeap* heap, Token* token) {
    if (!heap || !token || heap->heap_size >= heap->heap_capacity) return 0;
    Token* new_token = malloc(sizeof(Token));
    if (!new_token) return 0;
    if (strlen(token->value) >= MAX_TOKEN_LEN) {
        free(new_token);
        return 0;
    }
    *new_token = *token;
    heap->tokens_heap[heap->heap_size] = new_token;
    heap->heap_size++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int tokenize_string(const char* input, TokenStack* stack, TokenHeap* heap) {
    if (!input || !stack || !heap) return 0;
    
    const char* ptr = input;
    while (*ptr) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        Token token;
        memset(&token, 0, sizeof(Token));
        
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
            return 0;
        }
        
        if (!push_token(stack, token)) return 0;
        if (!add_to_heap(heap, &token)) return 0;
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    if (!push_token(stack, eof_token)) return 0;
    if (!add_to_heap(heap, &eof_token)) return 0;
    
    return 1;
}

void print_tokens(T