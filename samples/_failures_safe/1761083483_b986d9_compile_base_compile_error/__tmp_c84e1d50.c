//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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
    if (!str || !*str) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    return TOKEN_EOF;
}

int parse_input(const char* input, TokenList* token_list) {
    if (!input || !token_list) return 0;
    
    size_t input_len = strlen(input);
    size_t pos = 0;
    
    while (pos < input_len) {
        while (pos < input_len && isspace(input[pos])) pos++;
        if (pos >= input_len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_start = pos;
        size_t token_len = 0;
        
        char current = input[pos];
        TokenType type = classify_token(input + pos);
        
        if (type == TOKEN_NUMBER) {
            while (pos < input_len && isdigit(input[pos])) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
        } else if (type == TOKEN_IDENTIFIER) {
            while (pos < input_len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
        } else if (type == TOKEN_STRING) {
            pos++;
            while (pos < input_len && input[pos] != '"') {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            if (pos < input_len && input[pos] == '"') pos++;
        } else if (type == TOKEN_OPERATOR) {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
            }
            pos++;
        } else {
            pos++;
            continue;
        }
        
        if (token_len > 0) {
            token.value[token_len] = '\0';
            token.type = type;
            if (!token_list_add(token_list, token)) {
                return 0;
            }
        }
    }
    
    return 1;
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
        printf("Token %zu: %s -> '%s'\n", i + 1, type_str,