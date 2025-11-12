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
    if (initial_capacity == 0) return NULL;
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
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
    if (!list) return 0;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) return 0;
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
    if (!str || !str[0]) return TOKEN_IDENTIFIER;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, TokenList* token_list) {
    if (!input || !token_list) return 0;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {0};
        size_t token_start = i;
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = i + 1;
            while (j < len && input[j] != '"') j++;
            if (j >= len) return 0;
            
            size_t token_len = j - i + 1;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            i = j + 1;
        }
        else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = i;
            while (j < len && is_operator_char(input[j]) && (j - i) < MAX_TOKEN_LEN - 1) j++;
            
            size_t token_len = j - i;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            i = j;
        }
        else if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = i;
            while (j < len && (isdigit(input[j]) || input[j] == '.') && (j - i) < MAX_TOKEN_LEN - 1) j++;
            
            size_t token_len = j - i;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            i = j;
        }
        else if (isalpha(input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t j = i;
            while (j < len && (isalnum(input[j]) || input[j] == '_') && (j - i) < MAX_TOKEN_LEN - 1) j++;
            
            size_t token_len = j - i;
            if (token_len >= MAX_TOKEN_LEN) return 0;
            strncpy(token.value, input + i, token_len);
            token.value[token_len] = '\0';
            i = j;
        }
        else {
            i++;
            continue;
        }
        
        if (!token_list_add(token_list, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};