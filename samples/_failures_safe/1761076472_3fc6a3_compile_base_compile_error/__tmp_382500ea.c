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
    
    if (list->count < list->capacity) {
        list->tokens[list->count] = token;
        list->count++;
        return 1;
    }
    return 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    size_t start = *pos;
    size_t len = 0;
    
    while (input[*pos] && isdigit((unsigned char)input[*pos])) {
        (*pos)++;
        len++;
    }
    
    if (len > 0 && len < MAX_TOKEN_LEN) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    size_t start = *pos;
    size_t len = 0;
    
    if (input[*pos] && (isalpha((unsigned char)input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        len++;
        
        while (input[*pos] && (isalnum((unsigned char)input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
            len++;
        }
        
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(token->value, &input[start], len);
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return 1;
        }
    }
    return 0;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    if (input[*pos] == '"') {
        size_t start = *pos + 1;
        size_t len = 0;
        (*pos)++;
        
        while (input[*pos] && input[*pos] != '"') {
            (*pos)++;
            len++;
        }
        
        if (input[*pos] == '"') {
            if (len < MAX_TOKEN_LEN) {
                strncpy(token->value, &input[start], len);
                token->value[len] = '\0';
                token->type = TOKEN_STRING;
                (*pos)++;
                return 1;
            }
        }
    }
    return 0;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    if (!input || !pos || !token) return 0;
    if (input[*pos] && is_operator_char(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    return 0;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    TokenList* list = token_list_create();
    if (!list) return NULL;
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    
    while (pos < input_len && list->count < MAX_TOKENS) {
        while (pos < input_len && isspace((unsigned char)input[pos])) pos++;
        if (pos >= input_len) break;
        
        Token token;
        memset(&token, 0, sizeof(token));
        int parsed =