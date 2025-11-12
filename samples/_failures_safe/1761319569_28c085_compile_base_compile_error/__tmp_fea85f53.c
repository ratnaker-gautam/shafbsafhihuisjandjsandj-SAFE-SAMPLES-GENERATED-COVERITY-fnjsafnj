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
    int line;
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
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    int line = 1;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && tokens->count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) {
            if (input[i] == '\n') line++;
            i++;
        }
        
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, "", line};
        TokenType type = classify_token(input + i);
        token.type = type;
        
        size_t token_len = 0;
        
        switch (type) {
            case TOKEN_NUMBER:
                while (i < len && isdigit(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i++];
                }
                break;
                
            case TOKEN_IDENTIFIER:
                while (i < len && (isalnum(input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i++];
                }
                break;
                
            case TOKEN_OPERATOR:
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i++];
                }
                break;
                
            case TOKEN_STRING:
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i++];
                }
                while (i < len && input[i] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i++];
                }
                if (i < len && input[i] == '"' && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i++];
                }
                break;
                
            default:
                i++;
                continue;
        }
        
        token.value[token_len] = '\0';
        
        if (token_len > 0 && !token_list_add(tokens, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line};
    token_list_add(tokens, eof_token);
    
    return 1;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        printf("Line %d: ", token->line);
        
        switch (token->type) {
            case TOKEN_NUMBER: printf("NUMBER"); break;
            case TOKEN_IDENTIFIER: printf("