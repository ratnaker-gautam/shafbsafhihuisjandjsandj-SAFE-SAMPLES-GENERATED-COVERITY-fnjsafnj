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
    int column;
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
    
    array->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!array->tokens) {
        free(array);
        return NULL;
    }
    
    array->capacity = initial_capacity;
    array->size = 0;
    return array;
}

void destroy_token_array(TokenArray* array) {
    if (array) {
        free(array->tokens);
        free(array);
    }
}

int token_array_push(TokenArray* array, Token token) {
    if (!array || array->size >= MAX_TOKENS) return 0;
    
    if (array->size >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(array->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        
        array->tokens = new_tokens;
        array->capacity = new_capacity;
    }
    
    array->tokens[array->size] = token;
    array->size++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenArray* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenArray* tokens = create_token_array(16);
    if (!tokens) return NULL;
    
    int line = 1;
    int column = 1;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i < input_len && tokens->size < MAX_TOKENS; ) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (i >= input_len) break;
        
        Token token = {0};
        token.line = line;
        token.column = column;
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        } else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = 0;
            i++;
            column++;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
        } else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && is_operator_char(input[i])) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
        } else {
            i++;
            column++;
            continue;
        }
        
        if (!token_array_push(tokens, token)) {
            destroy_token_array(tokens);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line, column};
    token_array_push(tokens, eof_token);
    
    return tokens;
}

void print_tokens(const TokenArray* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->size; i++) {
        const Token* token = &tokens->tokens[i];
        const char* type_str = "";
        
        switch (token->type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TO