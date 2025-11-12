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
    if (!list) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) return 0;
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
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

void tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    size_t i = 0;
    char buffer[MAX_TOKEN_LEN];
    
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        while (isspace((unsigned char)input[i])) i++;
        
        if (input[i] == '\0') break;
        
        Token token;
        memset(&token, 0, sizeof(token));
        
        if (isdigit((unsigned char)input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                buffer[j++] = input[i++];
            }
            buffer[j] = '\0';
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                buffer[j++] = input[i++];
            }
            buffer[j] = '\0';
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = 0;
            while (is_operator_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                buffer[j++] = input[i++];
            }
            buffer[j] = '\0';
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = 0;
            i++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                buffer[j++] = input[i++];
            }
            if (input[i] == '"') i++;
            buffer[j] = '\0';
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else {
            i++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) {
            break;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str;
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;