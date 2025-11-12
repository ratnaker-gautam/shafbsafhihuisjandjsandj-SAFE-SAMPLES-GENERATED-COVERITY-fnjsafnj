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
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) return 0;
    list->tokens[list->count].type = type;
    strncpy(list->tokens[list->count].value, value, MAX_TOKEN_LEN - 1);
    list->tokens[list->count].value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, char* buffer) {
    const char* p = *input;
    size_t i = 0;
    while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
        buffer[i++] = *p++;
    }
    buffer[i] = '\0';
    if (i > 0) {
        *input = p;
        return 1;
    }
    return 0;
}

int parse_identifier(const char** input, char* buffer) {
    const char* p = *input;
    size_t i = 0;
    if (isalpha(*p) || *p == '_') {
        buffer[i++] = *p++;
        while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = *p++;
        }
        buffer[i] = '\0';
        *input = p;
        return 1;
    }
    return 0;
}

int parse_operator(const char** input, char* buffer) {
    const char* p = *input;
    size_t i = 0;
    if (is_operator_char(*p)) {
        buffer[i++] = *p++;
        if (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = *p++;
        }
        buffer[i] = '\0';
        *input = p;
        return 1;
    }
    return 0;
}

int parse_string(const char** input, char* buffer) {
    const char* p = *input;
    size_t i = 0;
    if (*p == '"') {
        p++;
        while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
            buffer[i++] = *p++;
        }
        if (*p == '"') {
            buffer[i] = '\0';
            *input = p + 1;
            return 1;
        }
    }
    return 0;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    TokenList* list = token_list_create();
    if (!list) return NULL;
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];
    while (*p != '\0' && list->count < MAX_TOKENS) {
        while (isspace(*p)) p++;
        if (*p == '\0') break;
        if (parse_number(&p, buffer)) {
            if (!token_list_add(list, TOKEN_NUMBER, buffer)) break;
        } else if (parse_identifier(&p, buffer)) {
            if (!token_list_add(list, TOKEN_IDENTIFIER, buffer)) break;
        } else if (parse_operator(&p, buffer)) {
            if (!token_list_add(list, TOKEN_OPERATOR, buffer)) break;
        } else if (parse_string(&p, buffer)) {
            if (!token_list_add(list, TOKEN_STRING, buffer)) break;
        } else {
            break;
        }
    }
    if (list->count < MAX_TOKENS) {
        token