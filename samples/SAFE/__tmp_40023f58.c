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

TokenList* token_list_create(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->capacity = 10;
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

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
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

TokenType classify_token(const char* value) {
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    size_t i = 0;
    
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        if (isspace(input[i])) {
            i++;
            continue;
        }
        
        if (is_operator_char(input[i])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), ""};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                if (!token_list_add(tokens, token)) return 0;
                buf_pos = 0;
            }
            
            buffer[0] = input[i];
            buffer[1] = '\0';
            Token token = {TOKEN_OPERATOR, ""};
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            if (!token_list_add(tokens, token)) return 0;
            i++;
            continue;
        }
        
        if (input[i] == '"') {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), ""};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                if (!token_list_add(tokens, token)) return 0;
                buf_pos = 0;
            }
            
            buffer[buf_pos++] = input[i++];
            while (input[i] != '\0' && input[i] != '"' && buf_pos < MAX_TOKEN_LEN - 1) {
                buffer[buf_pos++] = input[i++];
            }
            if (input[i] == '"') {
                buffer[buf_pos++] = input[i++];
            }
            buffer[buf_pos] = '\0';
            Token token = {TOKEN_STRING, ""};
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            if (!token_list_add(tokens, token)) return 0;
            buf_pos = 0;
            continue;
        }
        
        if (buf_pos >= MAX_TOKEN_LEN - 1) {
            buffer[buf_pos] = '\0';
            Token token = {classify_token(buffer), ""};
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            if (!token_list_add(tokens, token)) return 0;
            buf_pos = 0;
        }
        
        buffer[buf_pos++] = input[i++];
    }
    
    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        Token token = {classify_token(buffer), ""};
        strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
        if (!token_list_add(tokens, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, "EOF"};
    return token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i =