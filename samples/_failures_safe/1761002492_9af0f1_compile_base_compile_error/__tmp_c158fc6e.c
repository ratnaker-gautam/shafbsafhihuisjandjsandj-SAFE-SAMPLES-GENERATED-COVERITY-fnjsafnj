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
    if (initial_capacity == 0 || initial_capacity > MAX_TOKENS) return NULL;
    
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
        if (new_capacity <= array->capacity) return 0;
        
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

TokenType classify_token(const char* str) {
    if (!str || str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    
    if (str[0] == '"') return TOKEN_STRING;
    
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenArray* tokens) {
    if (!input || !tokens) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len && tokens->count < MAX_TOKENS) {
        while (pos < len && isspace((unsigned char)input[pos])) pos++;
        if (pos >= len) break;
        
        Token token;
        size_t token_len = 0;
        TokenType type = classify_token(input + pos);
        
        if (type == TOKEN_NUMBER) {
            while (pos < len && isdigit((unsigned char)input[pos]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos++];
            }
        } else if (type == TOKEN_IDENTIFIER) {
            while (pos < len && (isalnum((unsigned char)input[pos]) || input[pos] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos++];
            }
        } else if (type == TOKEN_OPERATOR) {
            token.value[token_len++] = input[pos++];
            if (pos < len && is_operator_char(input[pos]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos++];
            }
        } else if (type == TOKEN_STRING) {
            token.value[token_len++] = input[pos++];
            while (pos < len && input[pos] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos++];
            }
            if (pos < len && input[pos] == '"') {
                token.value[token_len++] = input[pos++];
            }
        } else {
            break;
        }
        
        if (token_len > 0) {
            token.value[token_len] = '\0';
            token.type = type;
            if (!token_array_push(tokens, token)) return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_array_push(tokens, eof_token);
    
    return 1;
}

void print_tokens(const TokenArray* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type