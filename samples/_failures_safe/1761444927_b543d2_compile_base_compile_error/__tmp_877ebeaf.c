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
    if (capacity == 0 || capacity > MAX_TOKENS) {
        return NULL;
    }
    
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) {
        return NULL;
    }
    
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
    if (!stack || stack->count >= stack->capacity) {
        return 0;
    }
    
    if (strlen(token.value) >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    stack->tokens[stack->count] = token;
    stack->count++;
    return 1;
}

int pop_token(TokenStack* stack, Token* result) {
    if (!stack || !result || stack->count == 0) {
        return 0;
    }
    
    stack->count--;
    *result = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        return TOKEN_NUMBER;
    } else if (is_operator(str[0])) {
        return TOKEN_OPERATOR;
    } else if (str[0] == '"') {
        return TOKEN_STRING;
    } else if (isalpha(str[0])) {
        return TOKEN_IDENTIFIER;
    }
    return TOKEN_EOF;
}

int tokenize_expression(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > 1024) {
        return 0;
    }
    
    *tokens = malloc(MAX_TOKENS * sizeof(Token));
    if (!*tokens) {
        return 0;
    }
    
    *token_count = 0;
    size_t i = 0;
    
    while (i < len && *token_count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        Token token;
        size_t j = 0;
        
        if (isdigit(input[i])) {
            while (i < len && (isdigit(input[i]) || input[i] == '.')) {
                if (j < MAX_TOKEN_LEN - 1) {
                    token.value[j++] = input[i++];
                } else {
                    i++;
                }
            }
            token.value[j] = '\0';
            token.type = TOKEN_NUMBER;
        } else if (is_operator(input[i])) {
            if (j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            token.type = TOKEN_OPERATOR;
        } else if (input[i] == '"') {
            token.value[j++] = input[i++];
            while (i < len && input[i] != '"' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            if (i < len && input[i] == '"' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            token.type = TOKEN_STRING;
        } else if (isalpha(input[i])) {
            while (i < len && (isalnum(input[i]) || input[i] == '_')) {
                if (j < MAX_TOKEN_LEN - 1) {
                    token.value[j++] = input[i++];
                } else {
                    i++;
                }
            }
            token.value[j] = '\0';
            token.type = TOKEN_IDENTIFIER;
        } else {
            i++;
            continue;
        }
        
        if (j > 0) {
            (*tokens)[*token_count] = token;
            (*token_count)++;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.value, "EOF");
    (*tokens)[*token_count] = eof_token;
    (*token_count)++;
    
    return 1;
}

void print_tokens(const Token* tokens, size_t count) {
    if (!tokens || count == 0) {