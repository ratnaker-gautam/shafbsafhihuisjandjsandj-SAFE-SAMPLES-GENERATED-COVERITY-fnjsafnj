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
    if (!str || !*str) return TOKEN_EOF;
    
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list = token_list_create(10);
    if (!list) return NULL;
    
    const char* p = input;
    while (*p && list->count < MAX_TOKENS) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t len = 0;
        
        token.type = classify_token(p);
        
        if (token.type == TOKEN_NUMBER) {
            while (isdigit((unsigned char)*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (token.type == TOKEN_IDENTIFIER) {
            while ((isalnum((unsigned char)*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (token.type == TOKEN_OPERATOR) {
            token.value[len++] = *p++;
            if (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
        } else if (token.type == TOKEN_STRING) {
            token.value[len++] = *p++;
            while (*p != '"' && *p && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *p++;
            }
            if (*p == '"') {
                token.value[len++] = *p++;
            }
        } else {
            break;
        }
        
        token.value[len] = '\0';
        if (!token_list_add(list, token)) break;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(list, eof_token);
    
    return list;
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const char* type_str = "";
        switch (list->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %zu: %s -> '%s'\n", i, type_str, list->tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    
    printf("