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
    if (!str || !str[0]) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int parse_token(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    
    size_t start = *pos;
    size_t len = strlen(input);
    
    while (start < len && isspace(input[start])) {
        start++;
    }
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        *pos = start;
        return 1;
    }
    
    token->type = classify_token(input + start);
    
    size_t token_len = 0;
    
    switch (token->type) {
        case TOKEN_NUMBER:
            while (start + token_len < len && isdigit(input[start + token_len])) {
                token_len++;
            }
            break;
            
        case TOKEN_OPERATOR:
            token_len = 1;
            break;
            
        case TOKEN_STRING:
            token_len = 1;
            while (start + token_len < len && input[start + token_len] != '"') {
                token_len++;
            }
            if (start + token_len < len && input[start + token_len] == '"') {
                token_len++;
            }
            break;
            
        case TOKEN_IDENTIFIER:
            while (start + token_len < len && (isalnum(input[start + token_len]) || input[start + token_len] == '_')) {
                token_len++;
            }
            break;
            
        default:
            return 0;
    }
    
    if (token_len >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, input + start, token_len);
    token->value[token_len] = '\0';
    *pos = start + token_len;
    
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list = token_list_create(10);
    if (!list) return NULL;
    
    size_t pos = 0;
    size_t len = strlen(input);
    
    while (pos < len) {
        Token token;
        if (!parse_token(input, &pos, &token)) {
            break;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (!token_list_add(list, token)) {
            token_list_destroy(list);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(list, eof_token);
    
    return list;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR