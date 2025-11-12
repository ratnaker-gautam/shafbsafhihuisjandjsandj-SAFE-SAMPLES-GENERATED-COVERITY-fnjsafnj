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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void token_array_init(TokenArray* arr, size_t initial_capacity) {
    arr->tokens = malloc(initial_capacity * sizeof(Token));
    if (arr->tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    arr->capacity = initial_capacity;
    arr->count = 0;
}

void token_array_push(TokenArray* arr, Token token) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity <= arr->capacity) {
            fprintf(stderr, "Capacity overflow\n");
            exit(1);
        }
        Token* new_tokens = realloc(arr->tokens, new_capacity * sizeof(Token));
        if (new_tokens == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
        arr->tokens = new_tokens;
        arr->capacity = new_capacity;
    }
    if (arr->count < MAX_TOKENS) {
        arr->tokens[arr->count] = token;
        arr->count++;
    }
}

void token_array_free(TokenArray* arr) {
    free(arr->tokens);
    arr->tokens = NULL;
    arr->capacity = 0;
    arr->count = 0;
}

int parse_number(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = 0;
    size_t input_len = strlen(input);
    
    while (*pos < input_len && isdigit(input[*pos])) {
        (*pos)++;
        len++;
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = 0;
    size_t input_len = strlen(input);
    
    if (*pos < input_len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        len++;
        (*pos)++;
        
        while (*pos < input_len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
            len++;
        }
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    size_t input_len = strlen(input);
    if (*pos >= input_len || input[*pos] != '"') return 0;
    
    size_t start = *pos + 1;
    size_t len = 0;
    (*pos)++;
    
    while (*pos < input_len && input[*pos] != '"' && input[*pos] != '\0') {
        (*pos)++;
        len++;
    }
    
    if (*pos < input_len && input[*pos] == '"') {
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            (*pos)++;
            return 1;
        }
    }
    return 0;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    size_t input_len = strlen(input);
    if (*pos < input_len && is_operator_char(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    return 0;
}

TokenArray tokenize(const char* input) {
    TokenArray tokens;
    token_array_init(&tokens, 10);
    
    if (input == NULL) {
        return tokens;
    }
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    
    while (pos < input_len && tokens.count < MAX_TOKENS) {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        
        Token token;
        int parsed = 0;
        
        if (parse_number(input, &pos, &token)) parsed = 1;
        else if (parse_string(input, &pos, &