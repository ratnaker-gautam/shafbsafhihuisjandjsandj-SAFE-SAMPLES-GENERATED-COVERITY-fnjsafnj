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
    int column;
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
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* value) {
    if (!value || strlen(value) == 0) return TOKEN_EOF;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    if (isalpha(value[0])) return TOKEN_IDENTIFIER;
    
    return TOKEN_IDENTIFIER;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    int line = 1;
    int column = 1;
    size_t input_len = strlen(input);
    size_t i = 0;
    
    while (i < input_len && tokens->count < MAX_TOKENS) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (i >= input_len) break;
        
        Token token = {0};
        token.line = line;
        token.column = column;
        
        if (isdigit(input[i])) {
            size_t j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
            token.type = TOKEN_NUMBER;
        }
        else if (is_operator_char(input[i])) {
            token.value[0] = input[i++];
            token.value[1] = '\0';
            column++;
            token.type = TOKEN_OPERATOR;
        }
        else if (input[i] == '"') {
            size_t j = 0;
            token.value[j++] = input[i++];
            column++;
            
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
                column++;
            }
            
            if (i < input_len && input[i] == '"') {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
            token.type = TOKEN_STRING;
        }
        else if (isalpha(input[i])) {
            size_t j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
                column++;
            }
            token.value[j] = '\0';
            token.type = TOKEN_IDENTIFIER;
        }
        else {
            i++;
            column++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line, column};
    token_list_add(tokens, eof_token);
    
    return 1;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type