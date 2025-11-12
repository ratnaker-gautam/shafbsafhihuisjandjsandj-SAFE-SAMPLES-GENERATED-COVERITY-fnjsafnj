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
    size_t count;
    size_t capacity;
} TokenList;

TokenList* token_list_create(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    list->capacity = 10;
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
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    if (list->count >= MAX_TOKENS) return 0;
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char** input) {
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

int parse_number(const char** input, Token* token) {
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
    if (!is_operator_char(**input)) return 0;
    
    token->value[0] = **input;
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*input)++;
    return 1;
}

int parse_string(const char** input, Token* token) {
    if (**input != '"') return 0;
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
    token->type = TOKEN_STRING;
    (*input)++;
    return 1;
}

TokenList* tokenize(const char* input) {
    TokenList* list = token_list_create();
    if (!list) return NULL;
    
    while (*input) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        int parsed = 0;
        
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        
        if (parsed) {
            if (!token_list_add(list, token)) {
                token_list_destroy(list);
                return NULL;
            }
        } else {
            token_list_destroy(list);
            return NULL;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    if (!token_list_add(list,