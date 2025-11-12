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
    
    if (strlen(token.value) >= MAX_TOKEN_LEN - 1) return 0;
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0]) && strlen(str) == 1) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list = token_list_create();
    if (!list) return NULL;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len && list->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        
        if (is_operator(input[i])) {
            token.value[0] = input[i];
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            i++;
        } else if (input[i] == '"') {
            size_t j = i + 1;
            while (j < len && input[j] != '"' && (j - i) < MAX_TOKEN_LEN - 1) j++;
            if (j < len && input[j] == '"') {
                size_t str_len = j - i + 1;
                if (str_len >= MAX_TOKEN_LEN) str_len = MAX_TOKEN_LEN - 1;
                strncpy(token.value, input + i, str_len);
                token.value[str_len] = '\0';
                token.type = TOKEN_STRING;
                i = j + 1;
            } else {
                break;
            }
        } else if (isalnum(input[i])) {
            while (i < len && (isalnum(input[i]) || input[i] == '_') && (i - start) < MAX_TOKEN_LEN - 1) i++;
            size_t token_len = i - start;
            if (token_len >= MAX_TOKEN_LEN) token_len = MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            token.type = classify_token(token.value);
        } else {
            i++;
            continue;
        }
        
        if (!token_list_add(list, token)) break;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(list, eof_token);
    
    return list;
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const Token* token = &list->tokens[i];
        const char* type_str = "";
        
        switch (token->type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        
        printf("Token %zu: %s ->