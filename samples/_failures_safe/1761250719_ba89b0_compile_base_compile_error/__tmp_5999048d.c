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

TokenType classify_token(const char* str) {
    if (!str || !str[0]) return TOKEN_IDENTIFIER;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len) {
        while (pos < len && isspace((unsigned char)input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {0};
        size_t token_start = pos;
        
        if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            pos++;
            size_t i = 0;
            while (pos < len && input[pos] != '"' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
            if (pos < len && input[pos] == '"') pos++;
        } else if (is_operator_char(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (pos < len && is_operator_char(input[pos]) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        } else if (isdigit((unsigned char)input[pos])) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (pos < len && (isdigit((unsigned char)input[pos]) || input[pos] == '.') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while (pos < len && (isalnum((unsigned char)input[pos]) || input[pos] == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        } else {
            pos++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    return token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token