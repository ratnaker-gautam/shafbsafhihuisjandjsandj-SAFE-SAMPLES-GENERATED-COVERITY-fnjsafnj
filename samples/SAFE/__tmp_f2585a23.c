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
    int line;
    int column;
} Token;

typedef struct {
    Token* tokens;
    size_t size;
    size_t capacity;
} TokenList;

TokenList* token_list_create(void) {
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    list->size = 0;
    list->capacity = 10;
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
    if (!list || list->size >= MAX_TOKENS) return 0;
    if (list->size >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        if (new_cap > MAX_TOKENS) new_cap = MAX_TOKENS;
        Token* new_tokens = realloc(list->tokens, new_cap * sizeof(Token));
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_cap;
    }
    list->tokens[list->size] = token;
    list->size++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char* input, int* pos, Token* token) {
    int start = *pos;
    while (isdigit(input[*pos])) (*pos)++;
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char* input, int* pos, Token* token) {
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') (*pos)++;
    int len = *pos - start;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_string(const char* input, int* pos, Token* token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int start = *pos;
    while (input[*pos] && input[*pos] != '"') (*pos)++;
    if (input[*pos] != '"') return 0;
    int len = *pos - start;
    (*pos)++;
    if (len >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_operator(const char* input, int* pos, Token* token) {
    if (!is_operator_char(input[*pos])) return 0;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    token->type = TOKEN_OPERATOR;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    TokenList* list = token_list_create();
    if (!list) return NULL;
    int pos = 0;
    int line = 1;
    int column = 1;
    while (input[pos] && list->size < MAX_TOKENS) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        Token token = {0};
        token.line = line;
        token.column = column;
        int success = 0;
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            success = parse_identifier(input, &pos, &token);
        } else if (input[pos] == '"') {
            success = parse_string(input, &pos, &token);
        } else if (is_operator_char(input[pos])) {
            success = parse_operator(input, &pos, &token);
        }
        if (success) {
            if (!token_list_add(list, token)) break;
            column += (int)strlen(token.value);
        } else {
            pos++;
            column++;
        }
    }
    Token eof_token = {TOKEN_EOF, "", line, column};
    token_list_add(list, eof_token);