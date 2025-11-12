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
    size_t count;
    size_t capacity;
} TokenList;

TokenList* token_list_create(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

void token_list_destroy(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_grow(TokenList* list) {
    if (!list) return 0;
    
    size_t new_capacity = list->capacity * 2;
    if (new_capacity <= list->capacity) return 0;
    
    Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
    if (!new_tokens) return 0;
    
    list->tokens = new_tokens;
    list->capacity = new_capacity;
    return 1;
}

int token_list_add(TokenList* list, TokenType type, const char* value) {
    if (!list || !value) return 0;
    
    if (list->count >= list->capacity) {
        if (!token_list_grow(list)) return 0;
    }
    
    if (list->count >= MAX_TOKENS) return 0;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) return 0;
    
    Token* token = &list->tokens[list->count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create(16);
    if (!tokens) return NULL;
    
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        char current = input[i];
        
        if (isdigit(current)) {
            char number[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                number[j++] = input[i++];
            }
            number[j] = '\0';
            token_list_add(tokens, TOKEN_NUMBER, number);
        }
        else if (isalpha(current) || current == '_') {
            char identifier[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                identifier[j++] = input[i++];
            }
            identifier[j] = '\0';
            token_list_add(tokens, TOKEN_IDENTIFIER, identifier);
        }
        else if (current == '"') {
            char string[MAX_TOKEN_LEN] = {0};
            size_t j = 0;
            i++;
            
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                string[j++] = input[i++];
            }
            string[j] = '\0';
            if (i < len && input[i] == '"') {
                i++;
                token_list_add(tokens, TOKEN_STRING, string);
            }
        }
        else if (is_operator_char(current)) {
            char op[2] = {current, '\0'};
            token_list_add(tokens, TOKEN_OPERATOR, op);
            i++;
        }
        else {
            i++;
        }
    }
    
    token_list_add(tokens, TOKEN_EOF, "");
    return tokens;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        const char* type_str = "";
        
        switch (token->type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case