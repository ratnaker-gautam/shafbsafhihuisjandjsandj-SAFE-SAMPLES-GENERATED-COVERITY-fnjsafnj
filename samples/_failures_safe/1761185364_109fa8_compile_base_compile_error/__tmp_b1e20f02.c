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

int parse_number(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = 0;
    
    while (input[*pos] && isdigit(input[*pos])) {
        (*pos)++;
        len++;
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = 0;
    
    if (input[*pos] && (isalpha(input[*pos]) || input[*pos] == '_')) {
        len++;
        (*pos)++;
        
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            len++;
            (*pos)++;
        }
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    if (input[*pos] && is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        return 1;
    }
    return 0;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    if (input[*pos] != '"') return 0;
    
    size_t start = *pos;
    size_t len = 1;
    (*pos)++;
    
    while (input[*pos] && input[*pos] != '"') {
        len++;
        (*pos)++;
    }
    
    if (input[*pos] == '"') {
        len++;
        (*pos)++;
        
        if (len < MAX_TOKEN_LEN) {
            token->type = TOKEN_STRING;
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            return 1;
        }
    }
    
    return 0;
}

TokenArray* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenArray* tokens = create_token_array(10);
    if (!tokens) return NULL;
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    
    while (pos < input_len && tokens->count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_number(input, &pos, &token)) parsed = 1;
        else if (parse_string(input, &pos, &token)) parsed = 1;
        else if (parse_identifier(input, &pos, &token))