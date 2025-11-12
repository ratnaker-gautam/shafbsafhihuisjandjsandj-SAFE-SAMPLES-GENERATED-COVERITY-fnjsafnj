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
    
    array->tokens[array->count] = token;
    array->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || !str[0]) return TOKEN_EOF;
    
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int parse_string_literal(const char* input, size_t* pos, char* output) {
    if (!input || !pos || !output) return 0;
    if (input[*pos] != '"') return 0;
    
    size_t start = *pos + 1;
    size_t i = 0;
    
    while (input[start + i] && input[start + i] != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        output[i] = input[start + i];
        i++;
    }
    
    if (input[start + i] != '"') return 0;
    
    output[i] = '\0';
    *pos = start + i + 1;
    return 1;
}

TokenArray* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenArray* tokens = create_token_array(16);
    if (!tokens) return NULL;
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    
    while (pos < input_len && tokens->count < MAX_TOKENS) {
        while (pos < input_len && isspace((unsigned char)input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        Token token = {TOKEN_EOF, ""};
        TokenType type = classify_token(input + pos);
        
        if (type == TOKEN_STRING) {
            token.type = TOKEN_STRING;
            if (!parse_string_literal(input, &pos, token.value)) {
                destroy_token_array(tokens);
                return NULL;
            }
        } else if (type == TOKEN_NUMBER) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && 
                   isdigit((unsigned char)input[pos])) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        } else if (type == TOKEN_OPERATOR) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = input[pos++];
            token.value[1] = '\0';
        } else if (type == TOKEN_IDENTIFIER) {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && 
                   (isalnum((unsigned char)input[pos]) || input[pos] == '_')) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        } else {
            break;
        }
        
        if (!token_array_push(tokens, token)) {
            destroy_token_array(tokens);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_array_push