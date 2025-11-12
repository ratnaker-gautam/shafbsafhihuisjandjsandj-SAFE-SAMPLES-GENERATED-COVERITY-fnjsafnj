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
    if (!list || list->count >= MAX_TOKENS) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
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

void tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    size_t i = 0;
    char current_token[MAX_TOKEN_LEN] = {0};
    size_t token_pos = 0;
    
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }
        
        Token token = {0};
        
        if (isdigit((unsigned char)input[i])) {
            token.type = TOKEN_NUMBER;
            token_pos = 0;
            
            while (isdigit((unsigned char)input[i]) && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            current_token[token_pos] = '\0';
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else if (isalpha((unsigned char)input[i])) {
            token.type = TOKEN_IDENTIFIER;
            token_pos = 0;
            
            while (isalnum((unsigned char)input[i]) && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            current_token[token_pos] = '\0';
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = input[i++];
            token.value[1] = '\0';
        }
        else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            token_pos = 0;
            i++;
            
            while (input[i] != '"' && input[i] != '\0' && token_pos < MAX_TOKEN_LEN - 1) {
                current_token[token_pos++] = input[i++];
            }
            if (input[i] == '"') i++;
            current_token[token_pos] = '\0';
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
        }
        else {
            i++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) {
            break;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\"\n", token->value);
                break;