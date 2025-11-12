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
} TokenArray;

TokenArray* create_token_array(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenArray* array = malloc(sizeof(TokenArray));
    if (!array) return NULL;
    
    array->tokens = malloc(initial_capacity * sizeof(Token));
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

int resize_token_array(TokenArray* array) {
    if (!array) return 0;
    
    size_t new_capacity = array->capacity * 2;
    if (new_capacity <= array->capacity) return 0;
    
    Token* new_tokens = realloc(array->tokens, new_capacity * sizeof(Token));
    if (!new_tokens) return 0;
    
    array->tokens = new_tokens;
    array->capacity = new_capacity;
    return 1;
}

int add_token(TokenArray* array, TokenType type, const char* value) {
    if (!array || !value || array->count >= MAX_TOKENS) return 0;
    
    if (array->count >= array->capacity) {
        if (!resize_token_array(array)) return 0;
    }
    
    Token* token = &array->tokens[array->count];
    token->type = type;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    array->count++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize_expression(const char* input, TokenArray* tokens) {
    if (!input || !tokens) return;
    
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        if (isdigit(input[i])) {
            char number[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            add_token(tokens, TOKEN_NUMBER, number);
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            add_token(tokens, TOKEN_IDENTIFIER, identifier);
        }
        else if (is_operator(input[i])) {
            char op[2] = {input[i++], '\0'};
            add_token(tokens, TOKEN_OPERATOR, op);
        }
        else if (input[i] == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            i++;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
            }
            string[j] = '\0';
            
            if (i < len && input[i] == '"') {
                i++;
                add_token(tokens, TOKEN_STRING, string);
            }
        }
        else {
            i++;
        }
    }
    
    add_token(tokens, TOKEN_EOF, "");
}

void print_tokens(const TokenArray* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        printf("Token %zu: ", i);
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER(%s)", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER(%s)", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR(%s)", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING(\"%s\")", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF");
                break;