//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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
    size_t count;
    size_t capacity;
} TokenList;

TokenList* token_list_create(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->capacity = 16;
    list->count = 0;
    list->tokens = malloc(list->capacity * sizeof(Token));
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    return list;
}

void token_list_destroy(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (list->count < list->capacity) {
        list->tokens[list->count] = token;
        list->count++;
        return 1;
    }
    
    return 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char** input) {
    if (!input || !*input) return;
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int parse_number(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    
    const char* start = *input;
    int has_dot = 0;
    
    while (**input && (isdigit((unsigned char)**input) || **input == '.')) {
        if (**input == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len == 0 || len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    
    const char* start = *input;
    
    if (!isalpha((unsigned char)**input) && **input != '_') return 0;
    (*input)++;
    
    while (**input && (isalnum((unsigned char)**input) || **input == '_')) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    
    const char* start = *input;
    
    while (**input && is_operator_char(**input)) {
        (*input)++;
    }
    
    size_t len = *input - start;
    if (len == 0 || len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char** input, Token* token) {
    if (!input || !*input || !token || **input != '"') return 0;
    
    (*input)++;
    const char* start = *input;
    
    while (**input && **input != '"') {
        (*input)++;
    }
    
    if (**input != '"') return 0;
    
    size_t len = *input - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    memcpy(token->value, start, len);
    token->value[len] = '\0';
    (*input)++;
    token->type = TOKEN_STRING;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list = token_list_create();
    if (!list) return NULL;
    
    while (*input) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        int parsed = 0;
        
        if (isdigit((unsigned char