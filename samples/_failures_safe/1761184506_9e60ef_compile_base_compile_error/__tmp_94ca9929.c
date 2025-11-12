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
    
    stack->tokens = malloc(sizeof(Token) * capacity);
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || strlen(str) == 0) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') return TOKEN_STRING;
    
    if (is_operator(str[0]) && strlen(str) == 1) return TOKEN_OPERATOR;
    
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_expression(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 1024) return 0;
    
    *tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!*tokens) return 0;
    
    *token_count = 0;
    size_t pos = 0;
    
    while (pos < len && *token_count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_start = pos;
        
        if (input[pos] == '"') {
            size_t end = pos + 1;
            while (end < len && input[end] != '"') end++;
            if (end < len) {
                size_t str_len = end - pos - 1;
                if (str_len < MAX_TOKEN_LEN - 1) {
                    strncpy(token.value, input + pos + 1, str_len);
                    token.value[str_len] = '\0';
                    token.type = TOKEN_STRING;
                    pos = end + 1;
                }
            }
        } else if (is_operator(input[pos])) {
            token.value[0] = input[pos];
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            pos++;
        } else if (isdigit(input[pos])) {
            size_t end = pos;
            while (end < len && (isdigit(input[end]) || input[end] == '.')) end++;
            size_t num_len = end - pos;
            if (num_len < MAX_TOKEN_LEN) {
                strncpy(token.value, input + pos, num_len);
                token.value[num_len] = '\0';
                token.type = TOKEN_NUMBER;
                pos = end;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            size_t end = pos;
            while (end < len && (isalnum(input[end]) || input[end] == '_')) end++;
            size_t id_len = end - pos;
            if (id_len < MAX_TOKEN_LEN) {
                strncpy(token.value, input + pos, id_len);
                token.value[id_len] = '\0';
                token.type = TOKEN_IDENTIFIER;
                pos = end;
            }
        } else {
            pos++;
            continue;
        }
        
        if (token.type != TOKEN_EOF) {
            (*tokens)[*token_count] = token;
            (*token_count)++;
        }
    }
    
    return 1;
}

void print_tokens(const Token* tokens, size_t count) {
    if (!tokens) return;
    
    for (size_t i = 0; i < count; i++)