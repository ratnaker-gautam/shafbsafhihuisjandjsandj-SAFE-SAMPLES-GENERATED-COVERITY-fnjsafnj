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

int token_list_add(TokenList* list, TokenType type, const char* value) {
    if (!list || !value) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (list->count >= MAX_TOKENS) return 0;
    
    Token* token = &list->tokens[list->count];
    token->type = type;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input) {
    if (!input || !*input) return 0;
    const char* start = *input;
    while (isdigit(**input)) (*input)++;
    return (*input - start) > 0;
}

int parse_identifier(const char** input) {
    if (!input || !*input) return 0;
    const char* start = *input;
    if (isalpha(**input) || **input == '_') {
        (*input)++;
        while (isalnum(**input) || **input == '_') (*input)++;
    }
    return (*input - start) > 0;
}

int parse_string(const char** input) {
    if (!input || !*input || **input != '"') return 0;
    
    (*input)++;
    const char* start = *input;
    
    while (**input && **input != '"') (*input)++;
    if (**input != '"') return 0;
    
    (*input)++;
    return 1;
}

int parse_operator(const char** input) {
    if (!input || !*input || !is_operator_char(**input)) return 0;
    
    const char* start = *input;
    while (is_operator_char(**input)) (*input)++;
    
    return (*input - start) > 0;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create();
    if (!tokens) return NULL;
    
    while (*input) {
        while (isspace(*input)) input++;
        if (!*input) break;
        
        const char* start = input;
        
        if (parse_number(&input)) {
            char num[MAX_TOKEN_LEN];
            size_t len = input - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(num, start, len);
            num[len] = '\0';
            token_list_add(tokens, TOKEN_NUMBER, num);
        } else if (parse_identifier(&input)) {
            char ident[MAX_TOKEN_LEN];
            size_t len = input - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(ident, start, len);
            ident[len] = '\0';
            token_list_add(tokens, TOKEN_IDENTIFIER, ident);
        } else if (parse_string(&input)) {
            char str[MAX_TOKEN_LEN];
            size_t len = (input - 1) - (start + 1);
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(str, start + 1, len);
            str[len] = '\0';
            token_list_add(tokens, TOKEN_STRING, str);
        } else if (parse_operator(&input)) {
            char op[MAX_TOKEN_LEN];
            size_t len = input - start;
            if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
            strncpy(op, start, len);
            op[len] = '\0';