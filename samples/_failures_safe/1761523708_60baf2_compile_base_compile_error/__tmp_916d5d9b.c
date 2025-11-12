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
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len && tokens->count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_start = pos;
        size_t token_len = 0;
        
        if (isdigit(input[pos])) {
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            token.type = TOKEN_NUMBER;
        } else if (isalpha(input[pos])) {
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            token.type = TOKEN_IDENTIFIER;
        } else if (is_operator_char(input[pos])) {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
            }
            pos++;
            token.type = TOKEN_OPERATOR;
        } else if (input[pos] == '"') {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
            }
            pos++;
            
            while (pos < len && input[pos] != '"') {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            
            if (pos < len && input[pos] == '"') {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            token.type = TOKEN_STRING;
        } else {
            pos++;
            continue;
        }
        
        token.value[token_len] = '\0';
        if (!token_list_add(tokens, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return 1;
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
            case TOKEN_EOF: type_str