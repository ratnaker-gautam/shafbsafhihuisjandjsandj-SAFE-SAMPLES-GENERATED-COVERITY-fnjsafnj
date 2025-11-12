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

int resize_token_array(TokenArray* array, size_t new_capacity) {
    if (!array || new_capacity == 0) return 0;
    
    Token* new_tokens = realloc(array->tokens, sizeof(Token) * new_capacity);
    if (!new_tokens) return 0;
    
    array->tokens = new_tokens;
    array->capacity = new_capacity;
    return 1;
}

int add_token(TokenArray* array, TokenType type, const char* value) {
    if (!array || !value || strlen(value) >= MAX_TOKEN_LEN) return 0;
    
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (!resize_token_array(array, new_capacity)) return 0;
    }
    
    Token* token = &array->tokens[array->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    array->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenArray* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenArray* tokens = create_token_array(16);
    if (!tokens) return NULL;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        char current = input[pos];
        
        if (isdigit(current)) {
            char number[MAX_TOKEN_LEN] = {0};
            size_t num_len = 0;
            
            while (pos < len && isdigit(input[pos]) && num_len < MAX_TOKEN_LEN - 1) {
                number[num_len++] = input[pos++];
            }
            number[num_len] = '\0';
            
            if (!add_token(tokens, TOKEN_NUMBER, number)) {
                destroy_token_array(tokens);
                return NULL;
            }
        }
        else if (isalpha(current) || current == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            size_t id_len = 0;
            
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && id_len < MAX_TOKEN_LEN - 1) {
                identifier[id_len++] = input[pos++];
            }
            identifier[id_len] = '\0';
            
            if (!add_token(tokens, TOKEN_IDENTIFIER, identifier)) {
                destroy_token_array(tokens);
                return NULL;
            }
        }
        else if (current == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            size_t str_len = 0;
            pos++;
            
            while (pos < len && input[pos] != '"' && str_len < MAX_TOKEN_LEN - 1) {
                string[str_len++] = input[pos++];
            }
            
            if (pos < len && input[pos] == '"') {
                pos++;
                string[str_len] = '\0';
                
                if (!add_token(tokens, TOKEN_STRING, string)) {
                    destroy_token_array(tokens);
                    return NULL;
                }
            } else {
                destroy_token_array(tokens);
                return NULL;
            }
        }
        else if (is_operator_char(current)) {
            char op[2] = {current, '\0'};
            pos++;
            
            if (!add_token(tokens, TOKEN_OPERATOR, op)) {
                destroy_token_array(tokens);
                return NULL;
            }
        }
        else {
            pos++;
        }
    }
    
    if (!add_token(tokens, TOKEN_EOF, "")) {
        destroy_token_array(tokens);
        return NULL;
    }
    
    return tokens;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING