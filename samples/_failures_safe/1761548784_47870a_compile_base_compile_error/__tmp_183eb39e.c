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
    int line;
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

TokenType classify_token(const char* value) {
    if (!value || strlen(value) == 0) return TOKEN_EOF;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    if (value[0] == '"') return TOKEN_STRING;
    if (isalpha(value[0])) return TOKEN_IDENTIFIER;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    int line = 1;
    int in_string = 0;
    
    for (size_t i = 0; input[i] != '\0' && buf_pos < MAX_TOKEN_LEN - 1; i++) {
        char c = input[i];
        
        if (c == '\n') {
            line++;
            continue;
        }
        
        if (c == '"') {
            if (in_string) {
                if (buf_pos > 0) {
                    buffer[buf_pos] = '\0';
                    Token token = {TOKEN_STRING, "", line};
                    strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                    token.value[MAX_TOKEN_LEN - 1] = '\0';
                    if (!token_list_add(tokens, token)) return 0;
                    buf_pos = 0;
                }
                in_string = 0;
            } else {
                in_string = 1;
            }
            continue;
        }
        
        if (in_string) {
            buffer[buf_pos++] = c;
            continue;
        }
        
        if (isspace(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), "", line};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_list_add(tokens, token)) return 0;
                buf_pos = 0;
            }
            continue;
        }
        
        if (is_operator_char(c)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                Token token = {classify_token(buffer), "", line};
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                if (!token_list_add(tokens, token)) return 0;
                buf_pos = 0;
            }
            buffer[0] = c;
            buffer[1] = '\0';
            Token token = {TOKEN_OPERATOR, "", line};
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
            if (!token_list_add(tokens, token)) return 0;
            buf_pos = 0;
            continue;
        }
        
        buffer[buf_pos++] = c;
    }
    
    if (buf_pos > 0 && !in_string) {
        buffer[buf_pos] = '\0';
        Token token = {classify_token(buffer), "", line};