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

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t i = 0;
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        
        if (isdigit(input[i])) {
            while (isdigit(input[i])) i++;
            size_t len = i - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, len);
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
        } else if (isalpha(input[i])) {
            while (isalnum(input[i])) i++;
            size_t len = i - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, len);
            token.value[len] = '\0';
            token.type = TOKEN_IDENTIFIER;
        } else if (is_operator_char(input[i])) {
            size_t len = 1;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, len);
            token.value[len] = '\0';
            token.type = TOKEN_OPERATOR;
            i++;
        } else if (input[i] == '"') {
            i++;
            start = i;
            while (input[i] != '\0' && input[i] != '"') i++;
            if (input[i] == '"') {
                size_t len = i - start;
                if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
                strncpy(token.value, input + start, len);
                token.value[len] = '\0';
                token.type = TOKEN_STRING;
                i++;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
        
        if (!token_list_add(tokens, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    if (!token_list_add(tokens, eof_token)) return 0;
    
    return 1;
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
                printf("IDENTIFIER: %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\"\n