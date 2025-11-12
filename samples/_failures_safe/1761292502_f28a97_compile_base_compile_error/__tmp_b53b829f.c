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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int tokenize(const char* input, TokenList* list) {
    if (!input || !list) return 0;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token;
        size_t token_len = 0;
        
        if (isdigit(input[i])) {
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[token_len++] = input[i++];
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_NUMBER;
        }
        else if (isalpha(input[i])) {
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                token.value[token_len++] = input[i++];
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_IDENTIFIER;
        }
        else if (is_operator(input[i])) {
            token.value[0] = input[i++];
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
        }
        else if (input[i] == '"') {
            token.value[token_len++] = input[i++];
            while (i < len && token_len < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[token_len++] = input[i++];
            }
            if (i < len && input[i] == '"') {
                token.value[token_len++] = input[i++];
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_STRING;
        }
        else {
            i++;
            continue;
        }
        
        if (!token_list_add(list, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    return token_list_add(list, eof_token);
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
                break;
            case TOKEN_STRING:
                printf("STRING: %s\n", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if