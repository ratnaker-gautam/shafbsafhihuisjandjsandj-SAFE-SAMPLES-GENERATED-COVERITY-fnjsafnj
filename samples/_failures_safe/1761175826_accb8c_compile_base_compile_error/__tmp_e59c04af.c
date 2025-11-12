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

TokenList* token_list_create(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->capacity = 16;
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
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || !str[0]) return TOKEN_EOF;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (isalpha((unsigned char)str[0])) return TOKEN_IDENTIFIER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    return TOKEN_EOF;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create();
    if (!tokens) return NULL;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace((unsigned char)input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        
        if (isdigit((unsigned char)input[i])) {
            while (i < len && (isdigit((unsigned char)input[i]) || input[i] == '.')) {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
        } else if (isalpha((unsigned char)input[i])) {
            while (i < len && (isalnum((unsigned char)input[i]) || input[i] == '_')) {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
        } else if (input[i] == '"') {
            i++;
            while (i < len && input[i] != '"') {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
            if (i < len && input[i] == '"') i++;
        } else if (is_operator_char(input[i])) {
            while (i < len && is_operator_char(input[i])) {
                if (i - start >= MAX_TOKEN_LEN - 1) break;
                i++;
            }
        } else {
            i++;
            continue;
        }
        
        size_t token_len = i - start;
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            token.type = classify_token(token.value);
            
            if (!token_list_add(tokens, token)) {
                token_list_destroy(tokens);
                return NULL;
            }
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return tokens;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "";
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %zu: %s ('%