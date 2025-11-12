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
    
    if (strlen(token.value) >= MAX_TOKEN_LEN - 1) return 0;
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0]) && strlen(str) == 1) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    return TOKEN_IDENTIFIER;
}

int tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = i;
        
        if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = input[i];
            token.value[1] = '\0';
            i++;
        } else if (isdigit(input[i])) {
            token.type = TOKEN_NUMBER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        } else if (isalpha(input[i])) {
            token.type = TOKEN_IDENTIFIER;
            size_t j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        } else if (input[i] == '"') {
            token.type = TOKEN_STRING;
            size_t j = 0;
            token.value[j++] = input[i++];
            while (i < len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token.value[j++] = input[i++];
            }
            if (i < len && input[i] == '"') {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        } else {
            i++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    return token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "";
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %zu: %s -> '%s'\n", i, type_str, tokens->tokens[i].value);
    }
}

int main(void) {
    char input[1024