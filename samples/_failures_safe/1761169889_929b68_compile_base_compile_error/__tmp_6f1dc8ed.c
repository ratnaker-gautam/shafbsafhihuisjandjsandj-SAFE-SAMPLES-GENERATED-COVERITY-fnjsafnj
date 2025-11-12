//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap > MAX_TOKENS) new_cap = MAX_TOKENS;
        Token* new_tokens = realloc(list->tokens, new_cap * sizeof(Token));
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_cap;
    }
    if (list->count >= MAX_TOKENS) return 0;
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char* input, size_t* pos, Token* token) {
    size_t i = 0;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[(*pos)++];
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    size_t i = 0;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[(*pos)++];
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    size_t i = 0;
    (*pos)++;
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[(*pos)++];
    }
    if (input[*pos] != '"') return 0;
    (*pos)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    size_t i = 0;
    while (is_operator_char(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = input[(*pos)++];
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list = token_list_create();
    if (!list) return NULL;
    
    size_t pos = 0;
    size_t len = strlen(input);
    
    while (pos < len) {
        while (isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token;
        int success = 0;
        
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            success = parse_identifier(input, &pos, &token);
        } else if (input[pos] == '"') {
            success = parse_string(input, &pos, &token);
        } else if (is_operator_char(input[pos])) {
            success = parse_operator(input, &pos, &token);
        } else {
            pos++;
            continue;
        }
        
        if (success) {
            if (!token_list_add(list, token)) {
                token_list_destroy(list);
                return NULL;
            }
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    if (!token_list_add(list, eof_token)) {
        token_list_destroy(list);
        return NULL;
    }
    
    return list;
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        const Token* token =