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
    size_t capacity;
    size_t count;
} TokenList;

TokenList* token_list_create(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_TOKENS) return NULL;
    
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

void token_list_destroy(TokenList* list) {
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
        if (new_capacity <= list->capacity) return 0;
        
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
    if (!str || str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    if (str[0] == '"') return TOKEN_STRING;
    
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    const char* pos = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;
    
    while (*pos && tokens->count < MAX_TOKENS) {
        while (isspace(*pos)) pos++;
        if (!*pos) break;
        
        buf_index = 0;
        Token token;
        
        if (isdigit(*pos)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(*pos) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *pos++;
            }
        } else if (isalpha(*pos) || *pos == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(*pos) || *pos == '_') && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *pos++;
            }
        } else if (is_operator_char(*pos)) {
            token.type = TOKEN_OPERATOR;
            buffer[buf_index++] = *pos++;
        } else if (*pos == '"') {
            token.type = TOKEN_STRING;
            buffer[buf_index++] = *pos++;
            while (*pos != '"' && *pos != '\0' && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *pos++;
            }
            if (*pos == '"') {
                buffer[buf_index++] = *pos++;
            }
        } else {
            pos++;
            continue;
        }
        
        buffer[buf_index] = '\0';
        strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
        token.value[MAX_TOKEN_LEN - 1] = '\0';
        
        if (!token_list_add(tokens, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(tokens, eof_token);
    
    return 1;
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens->tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case