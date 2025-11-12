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
    size_t capacity;
    size_t count;
} TokenList;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void token_list_init(TokenList* list) {
    list->capacity = 10;
    list->count = 0;
    list->tokens = malloc(list->capacity * sizeof(Token));
    if (!list->tokens) exit(EXIT_FAILURE);
}

void token_list_add(TokenList* list, Token token) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        Token* new_tokens = realloc(list->tokens, list->capacity * sizeof(Token));
        if (!new_tokens) {
            free(list->tokens);
            exit(EXIT_FAILURE);
        }
        list->tokens = new_tokens;
    }
    list->tokens[list->count++] = token;
}

void token_list_free(TokenList* list) {
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int parse_number(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = strlen(input);
    while (*pos < len && isdigit(input[*pos])) (*pos)++;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    if (*pos - start == 0) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = strlen(input);
    while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) (*pos)++;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    if (*pos - start == 0) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    size_t len = strlen(input);
    if (*pos >= len) return 0;
    if (*pos >= MAX_TOKEN_LEN - 1) return 0;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    token->type = TOKEN_OPERATOR;
    (*pos)++;
    return 1;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    size_t len = strlen(input);
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    size_t start = *pos;
    while (*pos < len && input[*pos] != '"') (*pos)++;
    if (*pos >= len || input[*pos] != '"') return 0;
    if (*pos - start >= MAX_TOKEN_LEN) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_STRING;
    (*pos)++;
    return 1;
}

int tokenize(const char* input, TokenList* tokens) {
    size_t pos = 0;
    size_t len = strlen(input);
    
    while (pos < len) {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        
        Token token;
        int success = 0;
        
        if (isdigit(input[pos])) {
            success = parse_number(input, &pos, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            success = parse_identifier(input, &pos, &token);
        } else if (is_operator_char(input[pos])) {
            success = parse_operator(input, &pos, &token);
        } else if (input[pos] == '"') {
            success = parse_string(input, &pos, &token);
        } else {
            return 0;
        }
        
        if (!success) return 0;
        token_list_add(tokens, token);
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    return 1;
}

void print_tokens(const TokenList* tokens) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (size_t i = 0; i < tokens->count; i++) {
        printf("Token %