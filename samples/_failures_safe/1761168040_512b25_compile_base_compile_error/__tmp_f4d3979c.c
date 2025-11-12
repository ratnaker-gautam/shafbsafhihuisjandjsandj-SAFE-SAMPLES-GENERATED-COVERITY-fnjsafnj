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
    size_t capacity;
    size_t count;
} TokenList;

TokenList* create_token_list(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->capacity = initial_capacity;
    list->count = 0;
    return list;
}

void destroy_token_list(TokenList* list) {
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
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    strncpy(list->tokens[list->count].value, token.value, MAX_TOKEN_LEN - 1);
    list->tokens[list->count].value[MAX_TOKEN_LEN - 1] = '\0';
    list->tokens[list->count].type = token.type;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || strlen(str) == 0) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* token_list) {
    if (!input || !token_list) return 0;
    
    size_t len = strlen(input);
    size_t pos = 0;
    
    while (pos < len && token_list->count < MAX_TOKENS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        Token token = {TOKEN_EOF, ""};
        size_t start = pos;
        
        if (input[pos] == '"') {
            token.type = TOKEN_STRING;
            pos++;
            while (pos < len && input[pos] != '"') {
                if (pos - start >= MAX_TOKEN_LEN - 1) break;
                pos++;
            }
            if (pos < len && input[pos] == '"') pos++;
            size_t token_len = (pos - start < MAX_TOKEN_LEN - 1) ? pos - start : MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
        }
        else if (is_operator_char(input[pos])) {
            token.type = TOKEN_OPERATOR;
            size_t token_len = 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
            pos++;
        }
        else if (isdigit(input[pos])) {
            token.type = TOKEN_NUMBER;
            while (pos < len && (isdigit(input[pos]) || input[pos] == '.')) {
                if (pos - start >= MAX_TOKEN_LEN - 1) break;
                pos++;
            }
            size_t token_len = (pos - start < MAX_TOKEN_LEN - 1) ? pos - start : MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            token.type = TOKEN_IDENTIFIER;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (pos - start >= MAX_TOKEN_LEN - 1) break;
                pos++;
            }
            size_t token_len = (pos - start < MAX_TOKEN_LEN - 1) ? pos - start : MAX_TOKEN_LEN - 1;
            strncpy(token.value, input + start, token_len);
            token.value[token_len] = '\0';
        }
        else {
            pos++;