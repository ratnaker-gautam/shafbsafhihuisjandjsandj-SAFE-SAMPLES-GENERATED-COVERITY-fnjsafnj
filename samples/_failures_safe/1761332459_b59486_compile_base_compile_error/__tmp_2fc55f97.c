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

TokenList* create_token_list(size_t initial_capacity) {
    if (initial_capacity == 0) {
        return NULL;
    }
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) {
        return NULL;
    }
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

void destroy_token_list(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) {
        return 0;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) {
            new_capacity = MAX_TOKENS;
        }
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) {
            return 0;
        }
        
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

TokenType classify_token(const char* value) {
    if (value[0] == '\0') {
        return TOKEN_EOF;
    }
    
    if (isdigit(value[0])) {
        return TOKEN_NUMBER;
    }
    
    if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (value[0] == '"') {
        return TOKEN_STRING;
    }
    
    if (isalpha(value[0]) || value[0] == '_') {
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, TokenList* token_list) {
    if (!input || !token_list) {
        return 0;
    }
    
    size_t input_len = strlen(input);
    size_t pos = 0;
    
    while (pos < input_len && token_list->count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        Token token = {TOKEN_EOF, ""};
        size_t token_len = 0;
        
        if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            while (pos < input_len && token_len < MAX_TOKEN_LEN - 1 && 
                   (isdigit(input[pos]) || input[pos] == '.')) {
                token.value[token_len++] = input[pos++];
            }
        } else if (is_operator_char(input[pos])) {
            token.type = TOKEN_OPERATOR;
            while (pos < input_len && token_len < MAX_TOKEN_LEN - 1 && 
                   is_operator_char(input[pos])) {
                token.value[token_len++] = input[pos++];
            }
        } else if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            token.value[token_len++] = input[pos++];
            while (pos < input_len && token_len < MAX_TOKEN_LEN - 1 && 
                   input[pos] != '"') {
                token.value[token_len++] = input[pos++];
            }
            if (pos < input_len && input[pos] == '"') {
                token.value[token_len++] = input[pos++];
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            while (pos < input_len && token_len < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[pos]) || input[pos] == '_')) {
                token.value[token_len++] = input[pos++];
            }
        } else {
            pos++;
            continue;
        }
        
        token.value[token_len] = '\0';
        token.type = classify_token(token.value);
        
        if (!token_list_add(token_list, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(token_list, eof_token);
    
    return 1;
}

void print_tokens(const TokenList* list) {
    if (!list) {
        return;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        const char* type_str = "";
        switch (list