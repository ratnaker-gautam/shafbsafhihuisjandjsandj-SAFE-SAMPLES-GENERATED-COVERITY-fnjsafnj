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
    size_t count;
    size_t capacity;
} TokenList;

TokenList* token_list_create(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
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
        if (new_capacity <= list->capacity) return 0;
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (!str || str[0] == '\0') return TOKEN_EOF;
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (isalpha((unsigned char)str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int parse_string_literal(const char* input, size_t* pos, char* output) {
    if (!input || !pos || !output) return 0;
    
    size_t out_pos = 0;
    (*pos)++;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (out_pos >= MAX_TOKEN_LEN - 1) return 0;
        if (input[*pos] == '\\') {
            (*pos)++;
            if (input[*pos] == '\0') return 0;
        }
        output[out_pos++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    output[out_pos] = '\0';
    (*pos)++;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* tokens = token_list_create(16);
    if (!tokens) return NULL;
    
    size_t pos = 0;
    char current_token[MAX_TOKEN_LEN];
    size_t token_pos = 0;
    
    while (input[pos] != '\0') {
        if (isspace((unsigned char)input[pos])) {
            pos++;
            continue;
        }
        
        if (input[pos] == '"') {
            Token token;
            token.type = TOKEN_STRING;
            if (!parse_string_literal(input, &pos, token.value)) {
                token_list_destroy(tokens);
                return NULL;
            }
            if (!token_list_add(tokens, token)) {
                token_list_destroy(tokens);
                return NULL;
            }
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.value[0] = input[pos];
            token.value[1] = '\0';
            pos++;
            if (!token_list_add(tokens, token)) {
                token_list_destroy(tokens);
                return NULL;
            }
            continue;
        }
        
        token_pos = 0;
        while (input[pos] != '\0' && !isspace((unsigned char)input[pos]) && 
               !is_operator_char(input[pos]) && input[pos] != '"') {
            if (token_pos >= MAX_TOKEN_LEN - 1) {
                token_list_destroy(tokens);
                return NULL;
            }
            current_token[token_pos++] = input[pos++];
        }
        current_token[token_pos] = '\0';
        
        if (token_pos > 0) {
            Token token;
            token.type = classify_token(current_token);
            strncpy(token.value, current_token, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
            if (!token_list_add(tokens, token)) {
                token_list_destroy(tokens);
                return NULL;
            }
        }
    }
    
    Token eof_token