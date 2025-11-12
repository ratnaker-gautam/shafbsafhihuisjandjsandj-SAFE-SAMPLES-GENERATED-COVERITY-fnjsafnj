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

void skip_whitespace(const char** input) {
    if (!input || !*input) return;
    while (**input && isspace((unsigned char)**input)) {
        (*input)++;
    }
}

Token get_next_token(const char** input) {
    Token token = {TOKEN_EOF, ""};
    if (!input || !*input) return token;
    
    skip_whitespace(input);
    
    if (!**input) {
        return token;
    }
    
    char current = **input;
    
    if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (**input && i < MAX_TOKEN_LEN - 1 && isdigit((unsigned char)**input)) {
            token.value[i++] = *(*input)++;
        }
        token.value[i] = '\0';
    }
    else if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (**input && i < MAX_TOKEN_LEN - 1 && 
               (isalnum((unsigned char)**input) || **input == '_')) {
            token.value[i++] = *(*input)++;
        }
        token.value[i] = '\0';
    }
    else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        size_t i = 0;
        while (**input && i < MAX_TOKEN_LEN - 1 && is_operator_char(**input)) {
            token.value[i++] = *(*input)++;
        }
        token.value[i] = '\0';
    }
    else if (current == '"') {
        token.type = TOKEN_STRING;
        (*input)++;
        size_t i = 0;
        while (**input && **input != '"' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = *(*input)++;
        }
        token.value[i] = '\0';
        if (**input == '"') {
            (*input)++;
        }
    }
    else {
        token.type = TOKEN_OPERATOR;
        token.value[0] = *(*input)++;
        token.value[1] = '\0';
    }
    
    return token;
}

TokenList* tokenize_string(const char* input) {
    if (!input) return NULL;
    TokenList* tokens = token_list_create(10);
    if (!tokens) return NULL;
    
    const char* current = input;
    while (*current) {
        Token token = get_next_token(&current);
        if (token.type == TOKEN_EOF) break;
        
        if (!token_list_add(tokens, token)) {
            token_list_destroy(tokens);
            return NULL;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return tokens;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    printf("Enter