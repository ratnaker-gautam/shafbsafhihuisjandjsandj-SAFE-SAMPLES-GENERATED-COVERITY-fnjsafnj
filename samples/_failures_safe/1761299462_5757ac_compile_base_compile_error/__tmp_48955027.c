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

int is_operator(char c) {
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

int parse_string(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    if (**input != '"') return 0;
    (*input)++;
    size_t i = 0;
    while (**input && **input != '"' && i < MAX_TOKEN_LEN - 1) {
        token->value[i++] = *(*input)++;
    }
    if (**input != '"') return 0;
    (*input)++;
    token->value[i] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_identifier(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    if (!isalpha((unsigned char)**input) && **input != '_') return 0;
    size_t i = 0;
    while ((isalnum((unsigned char)**input) || **input == '_') && i < MAX_TOKEN_LEN - 1) {
        token->value[i++] = *(*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char** input, Token* token) {
    if (!input || !*input || !token) return 0;
    if (!is_operator(**input)) return 0;
    token->value[0] = *(*input)++;
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
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
        int parsed = 0;
        if (parse_number(&input, &token)) parsed = 1;
        else if (parse_string(&input, &token)) parsed = 1;
        else if (parse_identifier(&input, &token)) parsed = 1;
        else if (parse_operator(&input, &token)) parsed = 1;
        if (!parsed) {
            token_list_destroy(list);
            return NULL;
        }
        if (!token_list_add(list, token)) {
            token_list_destroy(list);
            return NULL;
        }
    }
    Token eof_token = {TOKEN_EOF, ""};
    if (!token_list_add(list, eof_token)) {
        token_list_destroy(list);
        return NULL;
    }
    return list;
}

void