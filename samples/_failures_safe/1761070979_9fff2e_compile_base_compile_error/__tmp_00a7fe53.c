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

void tokenize_expression(const char* input, TokenArray* tokens) {
    if (!input || !tokens) return;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {0};
        size_t token_len = 0;
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[token_len++] = input[i++];
            }
        } else if (isalpha(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                token.value[token_len++] = input[i++];
            }
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            i++;
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[token_len++] = input[i++];
            }
            if (i < len && input[i] == '"') i++;
        } else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && is_operator_char(input[i])) {
                token.value[token_len++] = input[i++];
            }
        } else {
            i++;
            continue;
        }
        
        if (token_len > 0) {
            token.value[token_len] = '\0';
            if (!token_array_push(tokens, token)) break;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_array_push(tokens, eof_token);
}

void print_tokens(const TokenArray* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\"\n", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main() {
    char input[1024];
    
    printf("Enter an expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] =