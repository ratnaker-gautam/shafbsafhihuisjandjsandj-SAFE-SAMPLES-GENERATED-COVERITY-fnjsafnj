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
    int line;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t size;
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
    array->size = 0;
    return array;
}

int token_array_push(TokenArray* array, Token token) {
    if (!array || !array->tokens) return 0;
    
    if (array->size >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity <= array->capacity) return 0;
        
        Token* new_tokens = realloc(array->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        
        array->tokens = new_tokens;
        array->capacity = new_capacity;
    }
    
    if (array->size < array->capacity) {
        array->tokens[array->size] = token;
        array->size++;
        return 1;
    }
    return 0;
}

void destroy_token_array(TokenArray* array) {
    if (array) {
        free(array->tokens);
        free(array);
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* value) {
    if (!value || strlen(value) == 0) return TOKEN_EOF;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    if (isalpha(value[0])) return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

TokenArray* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenArray* tokens = create_token_array(10);
    if (!tokens) return NULL;
    
    size_t len = strlen(input);
    size_t pos = 0;
    int line = 1;
    
    while (pos < len) {
        while (pos < len && isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, "", line};
        size_t token_start = pos;
        
        if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            size_t str_len = 0;
            pos++;
            
            while (pos < len && input[pos] != '"' && str_len < MAX_TOKEN_LEN - 1) {
                token.value[str_len++] = input[pos++];
            }
            
            if (pos < len && input[pos] == '"') {
                pos++;
            }
            token.value[str_len] = '\0';
        } else if (is_operator_char(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t op_len = 0;
            
            while (pos < len && is_operator_char(input[pos]) && op_len < MAX_TOKEN_LEN - 1) {
                token.value[op_len++] = input[pos++];
            }
            token.value[op_len] = '\0';
        } else if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            size_t num_len = 0;
            
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.') && num_len < MAX_TOKEN_LEN - 1) {
                token.value[num_len++] = input[pos++];
            }
            token.value[num_len] = '\0';
        } else if (isalpha(input[pos])) {
            token.type = TOKEN_IDENTIFIER;
            size_t id_len = 0;
            
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && id_len < MAX_TOKEN_LEN - 1) {
                token.value[id_len++] = input[pos++];
            }
            token.value[id_len] = '\0';
        } else {
            pos++;
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

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: