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
        if (new_capacity <= list->capacity) return 0;
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

TokenType classify_token(const char* str) {
    if (!str || !str[0]) return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int parse_input(const char* input, TokenList* token_list) {
    if (!input || !token_list) return 0;
    
    size_t i = 0;
    while (input[i] != '\0' && token_list->count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        Token token;
        size_t token_len = 0;
        memset(&token, 0, sizeof(Token));
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            token.value[token_len++] = input[i++];
            while (input[i] != '\0' && input[i] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            if (input[i] == '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            while (is_operator_char(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            while (isdigit(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else {
            i++;
            continue;
        }
        
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            token.value[token_len] = '\0';
            if (!token_list_add(token_list, token)) return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    if (!token_list_add(token_list, eof_token)) return 0;
    
    return 1;
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const Token* token = &list->tokens[i];
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token->value);