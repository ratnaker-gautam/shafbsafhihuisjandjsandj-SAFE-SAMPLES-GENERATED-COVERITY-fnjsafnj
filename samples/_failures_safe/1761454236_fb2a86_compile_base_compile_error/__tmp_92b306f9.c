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

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) return 0;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
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

int parse_number(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = 0;
    
    while (isdigit(input[*pos])) {
        (*pos)++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (len == 0) return 0;
    
    token->type = TOKEN_NUMBER;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    return 1;
}

int parse_identifier(const char* input, size_t* pos, Token* token) {
    size_t start = *pos;
    size_t len = 0;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    token->type = TOKEN_IDENTIFIER;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    return 1;
}

int parse_operator(const char* input, size_t* pos, Token* token) {
    if (!is_operator_char(input[*pos])) return 0;
    
    token->type = TOKEN_OPERATOR;
    token->value[0] = input[*pos];
    token->value[1] = '\0';
    (*pos)++;
    return 1;
}

int parse_string(const char* input, size_t* pos, Token* token) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    size_t start = *pos;
    size_t len = 0;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        (*pos)++;
        len++;
        if (len >= MAX_TOKEN_LEN - 1) break;
    }
    
    if (input[*pos] != '"') return 0;
    
    token->type = TOKEN_STRING;
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    (*pos)++;
    return 1;
}

TokenList* tokenize(const char* input) {
    if (!input) return NULL;
    
    TokenList* list = token_list_create();
    if (!list) return NULL;
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    
    while (pos < input_len) {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        
        Token token;
        int parsed = 0;
        
        if (parse_number(input, &pos, &token)) parsed = 1;
        else if (parse_string(input, &pos, &token)) parsed = 1;
        else if (parse_identifier(input, &pos, &token)) parsed = 1;
        else if (parse_operator(input, &pos, &token)) parsed = 1;
        
        if (parsed) {
            if (!token_list_add(list, token)) {
                token_list_destroy(list);
                return NULL;
            }
        } else {
            pos++;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(list, eof_token);
    
    return list;
}

void print_tokens(const