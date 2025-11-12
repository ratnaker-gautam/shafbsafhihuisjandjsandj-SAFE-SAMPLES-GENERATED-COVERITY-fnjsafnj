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

TokenList* token_list_create(size_t initial_capacity) {
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

void token_list_destroy(TokenList* list) {
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
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    TokenList* tokens = token_list_create(10);
    if (!tokens) return NULL;
    
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) i++;
        if (i >= len) break;
        
        Token token;
        size_t token_len = 0;
        memset(&token, 0, sizeof(Token));
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            while (i < len && input[i] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            if (i < len && input[i] == '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            while (i < len && is_operator_char(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (isdigit((unsigned char)input[i])) {
            token.type = TOKEN_NUMBER;
            while (i < len && (isdigit((unsigned char)input[i]) || input[i] == '.') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            token.type = TOKEN_IDENTIFIER;
            while (i < len && (isalnum((unsigned char)input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else {
            i++;
            continue;
        }
        
        if (token_len < MAX_TOKEN_LEN) {
            token.value[token_len] = '\0';
        } else {
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        token.type = classify_token(token.value);
        
        if (!token_list_add(tokens, token)) {
            token_list_destroy(tokens);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    if (!token_list_add(tokens, eof_token)) {
        token_list_destroy(tokens);
        return NULL;
    }
    
    return tokens;
}

const char* token_type