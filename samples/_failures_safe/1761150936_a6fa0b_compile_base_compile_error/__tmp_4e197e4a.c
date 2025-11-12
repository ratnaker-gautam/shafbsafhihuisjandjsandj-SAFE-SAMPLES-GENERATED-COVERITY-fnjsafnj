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

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int parse_input(const char* input, TokenList* list) {
    if (!input || !list) return 0;
    
    size_t i = 0;
    while (input[i] != '\0' && list->count < MAX_TOKENS) {
        while (isspace(input[i])) i++;
        if (input[i] == '\0') break;
        
        Token token;
        size_t token_len = 0;
        token.type = classify_token(&input[i]);
        
        if (token.type == TOKEN_NUMBER) {
            while (isdigit(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (token.type == TOKEN_IDENTIFIER) {
            while ((isalnum(input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (token.type == TOKEN_OPERATOR) {
            token.value[token_len++] = input[i++];
            if (is_operator_char(input[i]) && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
        } else if (token.type == TOKEN_STRING) {
            token.value[token_len++] = input[i++];
            while (input[i] != '"' && input[i] != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            if (input[i] == '"') {
                token.value[token_len++] = input[i++];
            }
        } else {
            break;
        }
        
        token.value[token_len] = '\0';
        if (!token_list_add(list, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(list, eof_token);
    
    return 1;
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const Token* token = &list->tokens[i];
        printf("Token %zu: ", i);
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER '%s'", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER '%s'", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR '%s'", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING '%s'", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF");
                break;
        }
        printf("\n");
    }
}

int main(void) {
    char input[256];
    printf("Enter expression to tokenize: ");
    
    if (!fgets