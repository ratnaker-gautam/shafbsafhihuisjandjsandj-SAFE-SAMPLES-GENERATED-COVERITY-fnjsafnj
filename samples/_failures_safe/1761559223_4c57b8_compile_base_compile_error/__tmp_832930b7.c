//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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
    int line;
    int column;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenList;

void token_list_init(TokenList* list) {
    list->capacity = 16;
    list->count = 0;
    list->tokens = malloc(list->capacity * sizeof(Token));
    if (!list->tokens) exit(EXIT_FAILURE);
}

void token_list_add(TokenList* list, Token token) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) exit(EXIT_FAILURE);
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    if (list->count < MAX_TOKENS) {
        list->tokens[list->count++] = token;
    }
}

void token_list_free(TokenList* list) {
    free(list->tokens);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char* input, int* pos, int line, int col, Token* token) {
    int start = *pos;
    while (isdigit(input[*pos])) (*pos)++;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_identifier(const char* input, int* pos, int line, int col, Token* token) {
    int start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') (*pos)++;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_operator(const char* input, int* pos, int line, int col, Token* token) {
    int start = *pos;
    while (is_operator_char(input[*pos])) (*pos)++;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char* input, int* pos, int line, int col, Token* token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    int start = *pos;
    while (input[*pos] && input[*pos] != '"') (*pos)++;
    if (input[*pos] != '"') return 0;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    (*pos)++;
    return 1;
}

void tokenize(const char* input, TokenList* tokens) {
    int pos = 0;
    int line = 1;
    int col = 1;
    
    while (input[pos] && tokens->count < MAX_TOKENS) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
            pos++;
            continue;
        }
        
        Token token;
        int success = 0;
        
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, line, col, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            success = parse_identifier(input, &pos, line, col, &token);
        } else if (is_operator_char(input[pos])) {
            success = parse_operator(input, &pos, line, col, &token);
        } else if (input[pos] == '"') {
            success = parse_string(input, &pos, line, col, &token);
        }
        
        if (success) {
            token_list_add(tokens, token);
            col += (int)strlen(token.value);
        } else {
            col++;
            pos++;
        }
    }
    
    Token e