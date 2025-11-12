//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
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
    if (!list) return 0;
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        if (new_capacity <= list->capacity) return 0;
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    if (list->count >= MAX_TOKENS) return 0;
    list->tokens[list->count] = token;
    list->count++;
    return 1;
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

int parse_number(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    char* end;
    double val = strtod(*input, &end);
    if (end == *input) return 0;
    int len = snprintf(token->value, MAX_TOKEN_LEN, "%.6g", val);
    if (len < 0 || len >= MAX_TOKEN_LEN) return 0;
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    size_t i = 0;
    while (**input && (isalnum((unsigned char)**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    size_t i = 0;
    while (**input && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_OPERATOR;
    return 1;
}

int parse_string(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    if (**input != '"') return 0;
    (*input)++;
    size_t i = 0;
    while (**input && **input != '"') {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    if (**input != '"') return 0;
    (*input)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    TokenList* list = token_list_create();
    if (!list) return NULL;
    while (*input) {
        skip_whitespace(&input);
        if (!*input) break;
        Token token = {0};
        int success = 0;
        if (isdigit((unsigned char)*input) || (*input == '-' && isdigit((unsigned char)*(input + 1)))) {
            success = parse_number(&input, &token);
        } else if (isalpha((unsigned char)*input) || *input == '_') {
            success = parse_identifier(&input, &token);
        } else if (is_operator_char(*input)) {
            success = parse_operator(&input, &token);
        } else if (*input == '"') {
            success = parse_string(&input, &token);
        } else {
            break;
        }
        if (success)