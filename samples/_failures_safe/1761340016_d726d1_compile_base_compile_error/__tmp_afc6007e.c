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
    size_t count;
    size_t capacity;
} TokenList;

TokenList* create_token_list(size_t initial_capacity) {
    if (initial_capacity == 0) {
        return NULL;
    }
    
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) {
        return NULL;
    }
    
    list->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!list->tokens) {
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

void destroy_token_list(TokenList* list) {
    if (list) {
        free(list->tokens);
        free(list);
    }
}

int token_list_add(TokenList* list, Token token) {
    if (!list || list->count >= MAX_TOKENS) {
        return 0;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > MAX_TOKENS) {
            new_capacity = MAX_TOKENS;
        }
        
        Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) {
            return 0;
        }
        
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

TokenType classify_token(const char* value) {
    if (value[0] == '\0') {
        return TOKEN_EOF;
    }
    
    if (isdigit(value[0])) {
        return TOKEN_NUMBER;
    }
    
    if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (value[0] == '"') {
        return TOKEN_STRING;
    }
    
    if (isalpha(value[0]) || value[0] == '_') {
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* token_list) {
    if (!input || !token_list) {
        return 0;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        return 1;
    }
    
    size_t pos = 0;
    
    while (pos < input_len && token_list->count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        Token token = {TOKEN_EOF, ""};
        size_t token_start = pos;
        size_t token_len = 0;
        
        if (isdigit(input[pos])) {
            while (pos < input_len && (isdigit(input[pos]) || input[pos] == '.')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_NUMBER;
        }
        else if (is_operator_char(input[pos])) {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
            }
            pos++;
            token.value[token_len] = '\0';
            token.type = TOKEN_OPERATOR;
        }
        else if (input[pos] == '"') {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
            }
            pos++;
            
            while (pos < input_len && input[pos] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
                pos++;
            }
            
            if (pos < input_len && input[pos] == '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[pos];
                pos++;
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_STRING;
        }
        else if (isalpha(input[pos]) || input[pos] == '_') {
            while (pos < input_len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos];
                }
                pos++;
            }
            token.value[token_len] = '\0';
            token.type = TOKEN_IDENTIFIER;
        }
        else {
            pos++;
            continue;
        }
        
        if (!token_list_add(token_list, token)) {
            return 0;
        }
    }