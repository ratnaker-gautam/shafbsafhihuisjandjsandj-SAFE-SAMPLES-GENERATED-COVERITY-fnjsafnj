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

int token_list_grow(TokenList* list) {
    if (!list) return 0;
    
    size_t new_capacity = list->capacity * 2;
    if (new_capacity <= list->capacity) return 0;
    
    Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
    if (!new_tokens) return 0;
    
    list->tokens = new_tokens;
    list->capacity = new_capacity;
    return 1;
}

int token_list_add(TokenList* list, Token token) {
    if (!list) return 0;
    
    if (list->count >= list->capacity) {
        if (!token_list_grow(list)) return 0;
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

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create(10);
    if (!tokens) return NULL;
    
    const char* ptr = input;
    
    while (*ptr) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        Token token;
        token.type = classify_token(ptr);
        size_t len = 0;
        
        switch (token.type) {
            case TOKEN_NUMBER:
                while (isdigit(ptr[len])) len++;
                break;
            case TOKEN_IDENTIFIER:
                while (isalnum(ptr[len]) || ptr[len] == '_') len++;
                break;
            case TOKEN_OPERATOR:
                len = 1;
                break;
            case TOKEN_STRING:
                len = 1;
                while (ptr[len] && ptr[len] != '"') len++;
                if (ptr[len] == '"') len++;
                break;
            default:
                ptr++;
                continue;
        }
        
        if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
        if (len > 0) {
            strncpy(token.value, ptr, len);
            token.value[len] = '\0';
            if (!token_list_add(tokens, token)) {
                token_list_destroy(tokens);
                return NULL;
            }
            ptr += len;
        } else {
            ptr++;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    token_list_add(tokens, eof_token);
    
    return tokens;
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
                printf("STRING: %s\n", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input