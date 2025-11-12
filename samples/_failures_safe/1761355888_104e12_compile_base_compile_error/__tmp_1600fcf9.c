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
    
    array->tokens[array->count] = token;
    array->count++;
    return 1;
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
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), "", line};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_array_push(tokens, token)) return 0;
                buf_pos = 0;
            }
            continue;
        }
        
        if (isspace(c)) {
            if (buf_pos > 0 && !in_string) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), "", line};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_array_push(tokens, token)) return 0;
                buf_pos = 0;
            }
            continue;
        }
        
        if (c == '"') {
            if (in_string) {
                if (buf_pos < MAX_TOKEN_LEN - 1) {
                    buffer[buf_pos++] = c;
                }
                buffer[buf_pos] = '\0';
                Token token = {TOKEN_STRING, "", line};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_array_push(tokens, token)) return 0;
                buf_pos = 0;
                in_string = 0;
            } else {
                if (buf_pos > 0) {
                    buffer[buf_pos] = '\0';
                    Token token = {classify_token(buffer), "", line};
                    strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                    token.value[MAX_TOKEN_LEN - 1] = '\0';
                    if (!token_array_push(tokens, token)) return 0;
                    buf_pos = 0;
                }
                buffer[buf_pos++] = c;
                in_string = 1;
            }
            continue;
        }
        
        if (!in_string && is_operator_char(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), "", line};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';