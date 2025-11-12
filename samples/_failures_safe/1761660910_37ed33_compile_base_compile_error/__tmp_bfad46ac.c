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
    size_t capacity;
    size_t count;
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
    
    list->capacity = initial_capacity;
    list->count = 0;
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
    
    if (strlen(token.value) >= MAX_TOKEN_LEN - 1) return 0;
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    
    if (str[0] == '"') return TOKEN_STRING;
    
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* token_list) {
    if (!input || !token_list) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len && token_list->count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_start = pos;
        
        if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            token.value[i++] = input[pos++];
            
            while (pos < len && input[pos] != '"' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            
            if (pos < len && input[pos] == '"') {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else if (is_operator_char(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (pos < len && is_operator_char(input[pos]) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = input[pos++];
            }
            token.value[i] = '\0';
        }
        else {
            pos++;
            continue;
        }
        
        if (!token_list_add(token_list, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(token_list, eof_token);
    
    return 1;
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const char* type_str = "";
        switch (list->tokens[i].type) {
            case TOKEN_N