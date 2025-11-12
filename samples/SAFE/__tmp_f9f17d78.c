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

int token_list_add(TokenList* list, TokenType type, const char* value) {
    if (!list || !value) return 0;
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap > MAX_TOKENS) new_cap = MAX_TOKENS;
        if (list->count >= new_cap) return 0;
        Token* new_tokens = realloc(list->tokens, new_cap * sizeof(Token));
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_cap;
    }
    if (strlen(value) >= MAX_TOKEN_LEN) return 0;
    list->tokens[list->count].type = type;
    strncpy(list->tokens[list->count].value, value, MAX_TOKEN_LEN - 1);
    list->tokens[list->count].value[MAX_TOKEN_LEN - 1] = '\0';
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char* input, size_t* pos, char* buffer) {
    size_t i = 0;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char* input, size_t* pos, char* buffer) {
    size_t i = 0;
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[i++] = input[(*pos)++];
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (i >= MAX_TOKEN_LEN - 1) return 0;
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char* input, size_t* pos, char* buffer) {
    size_t i = 0;
    if (is_operator_char(input[*pos])) {
        buffer[i++] = input[(*pos)++];
        if (is_operator_char(input[*pos])) {
            if (i >= MAX_TOKEN_LEN - 1) return 0;
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char* input, size_t* pos, char* buffer) {
    size_t i = 0;
    if (input[*pos] == '"') {
        (*pos)++;
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (i >= MAX_TOKEN_LEN - 1) return 0;
            buffer[i++] = input[(*pos)++];
        }
        if (input[*pos] != '"') return 0;
        (*pos)++;
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    TokenList* list = token_list_create();
    if (!list) return NULL;
    size_t pos = 0;
    char buffer[MAX_TOKEN_LEN];
    while (input[pos] != '\0' && list->count < MAX_TOKENS) {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        if (parse_number(input, &pos, buffer)) {
            if (!token_list_add(list, TOKEN_NUMBER, buffer)) break;
        } else if (parse_identifier(input, &pos, buffer)) {
            if (!token_list_add(list, TOKEN_IDENTIFIER, buffer)) break;
        } else if (parse_operator(input, &pos, buffer)) {
            if (!token_list_add(list, TOKEN_OPERATOR, buffer)) break;
        } else if (parse_string(input, &pos, buffer)) {
            if (!token_list_add(list, TOKEN_STRING, buffer)) break;
        } else {
            break