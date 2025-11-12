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
    TOKEN_EOF,
    TOKEN_ERROR
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

int token_array_push(TokenArray* array, Token token) {
    if (!array || array->count >= MAX_TOKENS) return 0;
    
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(array->tokens, new_capacity * sizeof(Token));
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

TokenType classify_token(const char* value) {
    if (!value || strlen(value) == 0) return TOKEN_ERROR;
    
    if (isdigit(value[0])) {
        for (size_t i = 0; i < strlen(value); i++) {
            if (!isdigit(value[i])) return TOKEN_ERROR;
        }
        return TOKEN_NUMBER;
    }
    
    if (value[0] == '"' && value[strlen(value)-1] == '"') {
        return TOKEN_STRING;
    }
    
    if (is_operator_char(value[0]) && strlen(value) == 1) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha(value[0])) {
        for (size_t i = 0; i < strlen(value); i++) {
            if (!isalnum(value[i])) return TOKEN_ERROR;
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_ERROR;
}

int tokenize_string(const char* input, TokenArray* tokens) {
    if (!input || !tokens) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    int line = 1;
    int in_string = 0;
    
    for (size_t i = 0; input[i] != '\0' && tokens->count < MAX_TOKENS; i++) {
        char c = input[i];
        
        if (c == '\n') {
            line++;
        }
        
        if (in_string) {
            if (c == '"') {
                if (buf_pos < MAX_TOKEN_LEN - 1) {
                    buffer[buf_pos++] = c;
                    buffer[buf_pos] = '\0';
                    
                    Token token;
                    token.type = TOKEN_STRING;
                    strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                    token.value[MAX_TOKEN_LEN - 1] = '\0';
                    token.line = line;
                    
                    if (!token_array_push(tokens, token)) return 0;
                    
                    buf_pos = 0;
                    in_string = 0;
                }
            } else {
                if (buf_pos < MAX_TOKEN_LEN - 1) {
                    buffer[buf_pos++] = c;
                }
            }
            continue;
        }
        
        if (isspace(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                Token token;
                token.type = classify_token(buffer);
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                token.line = line;
                
                if (token.type != TOKEN_ERROR) {
                    if (!token_array_push(tokens, token)) return 0;
                }
                
                buf_pos = 0;
            }
            continue;
        }
        
        if (c == '"') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                Token token;
                token.type = classify_token(buffer);
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                token.line = line;
                
                if (token.type != TOKEN_ERROR) {
                    if (!token_array