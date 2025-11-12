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

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* value) {
    if (value[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    
    if (value[0] == '"') return TOKEN_STRING;
    
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create(10);
    if (!tokens) return NULL;
    
    size_t i = 0;
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        
        if (input[i] == '\0') break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_len = 0;
        
        if (isdigit(input[i])) {
            while (isdigit(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_NUMBER;
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            while ((isalnum(input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_IDENTIFIER;
        }
        else if (input[i] == '"') {
            token.value[token_len++] = input[i++];
            while (input[i] != '"' && input[i] != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            if (input[i] == '"') {
                token.value[token_len++] = input[i++];
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_STRING;
        }
        else if (is_operator_char(input[i])) {
            token.value[token_len++] = input[i++];
            token.value[token_len] = '\0';
            token.type = TOKEN_OPERATOR;
        }
        else {
            i++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) {
            token_list_destroy(tokens);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return tokens;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str =