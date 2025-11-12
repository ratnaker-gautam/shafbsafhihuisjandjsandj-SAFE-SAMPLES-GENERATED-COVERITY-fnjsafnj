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
    size_t pos = 0;
    
    while (pos < input_len && tokens->size < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (pos >= input_len) break;
        
        Token token = {0};
        token.line = line;
        token.column = column;
        
        if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            token.value[i++] = input[pos++];
            column++;
            
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                token.value[i++] = input[pos++];
                column++;
            }
            
            if (pos < input_len && input[pos] == '"') {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else if (is_operator_char(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (pos < input_len && i < MAX_TOKEN_LEN - 1 && is_operator_char(input[pos])) {
                token.value[i++] = input[pos++];
                column++;
            }
            token.value[i] = '\0';
        } else {
            pos++;
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

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter