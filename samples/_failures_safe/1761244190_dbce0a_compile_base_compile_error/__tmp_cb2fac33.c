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
    int line;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t size;
} TokenList;

TokenList* create_token_list(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->capacity = initial_capacity;
    list->size = 0;
    return list;
}

void destroy_token_list(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_resize(TokenList* list, size_t new_capacity) {
    if (!list || new_capacity == 0) return 0;
    
    Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
    if (!new_tokens) return 0;
    
    list->tokens = new_tokens;
    list->capacity = new_capacity;
    return 1;
}

int token_list_add(TokenList* list, Token token) {
    if (!list) return 0;
    
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) return 0;
        if (!token_list_resize(list, new_capacity)) return 0;
    }
    
    if (list->size < list->capacity) {
        list->tokens[list->size] = token;
        list->size++;
        return 1;
    }
    
    return 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    int line = 1;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len) {
        while (i < len && isspace(input[i])) {
            if (input[i] == '\n') line++;
            i++;
        }
        
        if (i >= len) break;
        
        Token token = {0};
        token.line = line;
        
        if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && is_operator_char(input[i])) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        }
        else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = 0;
            i++;
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            if (i < len && input[i] == '"') i++;
        }
        else {
            i++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) {
            break;
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line};
    token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->size; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s (line %d)\n", token->value, token->line);
                break;
            case