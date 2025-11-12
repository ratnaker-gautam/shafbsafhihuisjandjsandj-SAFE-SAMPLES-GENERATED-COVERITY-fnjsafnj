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

TokenList* create_token_list(size_t initial_capacity) {
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

void destroy_token_list(TokenList* list) {
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
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    
    return TOKEN_EOF;
}

int parse_number(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    
    size_t i = *pos;
    size_t j = 0;
    size_t len = strlen(input);
    
    while (i < len && isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
        token->value[j++] = input[i++];
    }
    
    if (j > 0) {
        token->value[j] = '\0';
        token->type = TOKEN_NUMBER;
        *pos = i;
        return 1;
    }
    
    return 0;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    
    size_t i = *pos;
    size_t j = 0;
    size_t len = strlen(input);
    
    while (i < len && isalnum(input[i]) && j < MAX_TOKEN_LEN - 1) {
        token->value[j++] = input[i++];
    }
    
    if (j > 0) {
        token->value[j] = '\0';
        token->type = TOKEN_IDENTIFIER;
        *pos = i;
        return 1;
    }
    
    return 0;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    
    size_t i = *pos;
    size_t len = strlen(input);
    
    if (i < len && is_operator_char(input[i])) {
        token->value[0] = input[i];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        *pos = i + 1;
        return 1;
    }
    
    return 0;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    
    size_t i = *pos;
    size_t j = 0;
    size_t len = strlen(input);
    
    if (i >= len || input[i] != '"') return 0;
    
    i++;
    while (i < len && input[i] != '"' && j < MAX_TOKEN_LEN - 1) {
        token->value[j++] = input[i++];
    }
    
    if (i < len && input[i] == '"') {
        token->value[j] = '\0';
        token->type = TOKEN_STRING;
        *pos = i + 1;
        return 1;
    }
    
    return 0;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list