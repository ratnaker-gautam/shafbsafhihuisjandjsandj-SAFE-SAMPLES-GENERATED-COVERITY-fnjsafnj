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

int token_list_add(TokenList* list, TokenType type, const char* value) {
    if (!list || !value) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (list->count >= MAX_TOKENS) return 0;
    
    Token* token = &list->tokens[list->count];
    token->type = type;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create();
    if (!tokens) return NULL;
    
    const char* p = input;
    
    while (*p) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            char buffer[MAX_TOKEN_LEN];
            size_t i = 0;
            
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            buffer[i] = '\0';
            
            if (!token_list_add(tokens, TOKEN_NUMBER, buffer)) {
                token_list_destroy(tokens);
                return NULL;
            }
        }
        else if (isalpha(*p) || *p == '_') {
            char buffer[MAX_TOKEN_LEN];
            size_t i = 0;
            
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            buffer[i] = '\0';
            
            if (!token_list_add(tokens, TOKEN_IDENTIFIER, buffer)) {
                token_list_destroy(tokens);
                return NULL;
            }
        }
        else if (*p == '"') {
            char buffer[MAX_TOKEN_LEN];
            size_t i = 0;
            p++;
            
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            
            if (*p != '"') {
                token_list_destroy(tokens);
                return NULL;
            }
            
            p++;
            buffer[i] = '\0';
            
            if (!token_list_add(tokens, TOKEN_STRING, buffer)) {
                token_list_destroy(tokens);
                return NULL;
            }
        }
        else if (is_operator_char(*p)) {
            char buffer[3] = {*p, '\0', '\0'};
            
            if (p[1] && is_operator_char(p[1]) && 
                ((*p == '&' && p[1] == '&') || 
                 (*p == '|' && p[1] == '|') ||
                 (*p == '=' && p[1] == '=') ||
                 (*p == '!' && p[1] == '=') ||
                 (*p == '<' && p[1] == '=') ||
                 (*p == '>' && p[1] == '='))) {
                buffer[1] = p[1];
                buffer[2] = '\0';
                p += 2;
            } else {
                p++;
            }
            
            if (!token_list_add(tokens, TOKEN_OPERATOR, buffer)) {
                token_list_destroy(tokens);
                return NULL;
            }
        }
        else {
            token_list_destroy(tokens);
            return NULL;
        }
    }
    
    token_list_add(tokens, TOKEN_EOF, "");
    return tokens;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER