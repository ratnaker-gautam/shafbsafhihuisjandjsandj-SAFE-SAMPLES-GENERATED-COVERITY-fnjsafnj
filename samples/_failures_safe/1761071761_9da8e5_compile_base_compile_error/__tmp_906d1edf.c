//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    
    list->capacity = 16;
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
    
    TokenList* tokens = token_list_create();
    if (!tokens) return NULL;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len && tokens->count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t token_len = 0;
        
        token.type = classify_token(input + pos);
        
        if (token.type == TOKEN_NUMBER) {
            while (pos < len && token_len < MAX_TOKEN_LEN - 1 && isdigit(input[pos])) {
                token.value[token_len++] = input[pos++];
            }
        } else if (token.type == TOKEN_IDENTIFIER) {
            while (pos < len && token_len < MAX_TOKEN_LEN - 1 && (isalnum(input[pos]) || input[pos] == '_')) {
                token.value[token_len++] = input[pos++];
            }
        } else if (token.type == TOKEN_OPERATOR) {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos++];
            }
        } else if (token.type == TOKEN_STRING) {
            token.value[token_len++] = input[pos++];
            while (pos < len && token_len < MAX_TOKEN_LEN - 1 && input[pos] != '"') {
                token.value[token_len++] = input[pos++];
            }
            if (pos < len && input[pos] == '"') {
                token.value[token_len++] = input[pos++];
            }
        } else {
            pos++;
            continue;
        }
        
        token.value[token_len] = '\0';
        if (!token_list_add(tokens, token)) break;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return tokens;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "UNKNOWN";
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
    char input[102