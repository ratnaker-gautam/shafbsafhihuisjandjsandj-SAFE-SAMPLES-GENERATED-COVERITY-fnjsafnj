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
    if (!str || !str[0]) return TOKEN_EOF;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) i++;
        if (i >= len) break;
        
        Token token;
        memset(&token, 0, sizeof(token));
        size_t token_start = i;
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            i++;
            size_t j = 0;
            while (i < len && input[i] != '"' && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            if (i < len && input[i] == '"') i++;
        } else if (is_operator_char(input[i])) {
            token.type = TOKEN_OPERATOR;
            size_t j = 0;
            while (i < len && is_operator_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
        } else if (isalnum((unsigned char)input[i]) || input[i] == '_') {
            token.type = classify_token(&input[i]);
            size_t j = 0;
            while (i < len && (isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
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
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main() {
    char input