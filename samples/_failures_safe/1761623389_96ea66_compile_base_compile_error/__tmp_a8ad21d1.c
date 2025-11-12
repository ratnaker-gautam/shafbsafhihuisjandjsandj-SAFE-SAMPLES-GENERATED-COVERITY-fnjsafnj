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
    int line;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenArray;

TokenArray* create_token_array(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenArray* array = malloc(sizeof(TokenArray));
    if (!array) return NULL;
    
    array->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!array->tokens) {
        free(array);
        return NULL;
    }
    
    array->capacity = initial_capacity;
    array->count = 0;
    return array;
}

void destroy_token_array(TokenArray* array) {
    if (array) {
        free(array->tokens);
        free(array);
    }
}

int token_array_push(TokenArray* array, Token token) {
    if (!array || array->count >= MAX_TOKENS) return 0;
    
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(array->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        
        array->tokens = new_tokens;
        array->capacity = new_capacity;
    }
    
    if (array->count < array->capacity) {
        array->tokens[array->count] = token;
        array->count++;
        return 1;
    }
    
    return 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenArray* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenArray* tokens = create_token_array(16);
    if (!tokens) return NULL;
    
    int line = 1;
    const char* ptr = input;
    
    while (*ptr && tokens->count < MAX_TOKENS) {
        if (isspace((unsigned char)*ptr)) {
            if (*ptr == '\n') line++;
            ptr++;
            continue;
        }
        
        Token token = {0};
        token.line = line;
        
        if (isdigit((unsigned char)*ptr)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)*ptr) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        }
        else if (*ptr == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
            if (*ptr == '"') ptr++;
        }
        else if (is_operator_char(*ptr)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *ptr++;
            }
            token.value[i] = '\0';
        }
        else {
            ptr++;
            continue;
        }
        
        if (!token_array_push(tokens, token)) {
            destroy_token_array(tokens);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line};
    token_array_push(tokens, eof_token);
    
    return tokens;
}

void print_tokens(const TokenArray* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\" (line %d)\n", token->value, token->line);
                break;
            case TOKEN_EOF:
                printf