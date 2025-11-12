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
    
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    strncpy(list->tokens[list->count].value, token.value, MAX_TOKEN_LEN - 1);
    list->tokens[list->count].value[MAX_TOKEN_LEN - 1] = '\0';
    list->tokens[list->count].type = token.type;
    list->count++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize_expression(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    size_t i = 0;
    while (input[i] != '\0' && tokens->count < MAX_TOKENS) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }
        
        Token token;
        
        if (isdigit((unsigned char)input[i])) {
            size_t j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            token.type = TOKEN_NUMBER;
            token_list_add(tokens, token);
        }
        else if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            size_t j = 0;
            while ((isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            token.type = TOKEN_IDENTIFIER;
            token_list_add(tokens, token);
        }
        else if (is_operator(input[i])) {
            token.value[0] = input[i++];
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            token_list_add(tokens, token);
        }
        else if (input[i] == '"') {
            size_t j = 0;
            token.value[j++] = input[i++];
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 2) {
                token.value[j++] = input[i++];
            }
            if (input[i] == '"') {
                token.value[j++] = input[i++];
            }
            token.value[j] = '\0';
            token.type = TOKEN_STRING;
            token_list_add(tokens, token);
        }
        else {
            i++;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
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
        printf("Token %zu: %s ('%s')\n", i, type_str, tokens->tokens[i].value);
    }
}

int main(void) {
    char input[256];