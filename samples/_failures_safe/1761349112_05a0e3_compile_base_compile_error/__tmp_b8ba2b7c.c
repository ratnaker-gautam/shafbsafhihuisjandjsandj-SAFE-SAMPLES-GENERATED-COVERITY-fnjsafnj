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
    if (!list) return 0;
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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || strlen(str) == 0) return TOKEN_IDENTIFIER;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_start = i;
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = i + 1;
            while (j < len && input[j] != '"') j++;
            if (j >= len) return 0;
            
            size_t token_len = j - i + 1;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            i = j + 1;
        }
        else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = i;
            while (j < len && is_operator(input[j]) && (j - i) < MAX_TOKEN_LEN - 1) j++;
            
            size_t token_len = j - i;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            i = j;
        }
        else if (isalnum((unsigned char)input[i]) || input[i] == '_') {
            size_t j = i;
            while (j < len && (isalnum((unsigned char)input[j]) || input[j] == '_') && (j - i) < MAX_TOKEN_LEN - 1) j++;
            
            size_t token_len = j - i;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            token.type = classify_token(token.value);
            i = j;
        }
        else {
            i++;
            continue;
        }
        
        if (strlen(token.value) > 0 && !token_list_add(tokens, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    return token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "";
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str =