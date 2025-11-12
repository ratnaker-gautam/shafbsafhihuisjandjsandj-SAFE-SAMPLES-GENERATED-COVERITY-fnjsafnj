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
    if (!value || strlen(value) == 0) {
        return TOKEN_EOF;
    }
    
    if (isdigit(value[0])) {
        return TOKEN_NUMBER;
    }
    
    if (value[0] == '"') {
        return TOKEN_STRING;
    }
    
    if (is_operator_char(value[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (isalpha(value[0]) || value[0] == '_') {
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* tokens) {
    if (!input || !tokens) {
        return 0;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        return 1;
    }
    
    char buffer[MAX_TOKEN_LEN];
    size_t buffer_pos = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (isspace(input[i])) {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                Token token;
                token.type = classify_token(buffer);
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                
                if (!token_list_add(tokens, token)) {
                    return 0;
                }
                buffer_pos = 0;
            }
            continue;
        }
        
        if (buffer_pos >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        
        if (is_operator_char(input[i]) && buffer_pos == 0) {
            buffer[0] = input[i];
            buffer[1] = '\0';
            Token token;
            token.type = TOKEN_OPERATOR;
            strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
            token.value[MAX_TOKEN_LEN - 1] = '\0';
            
            if (!token_list_add(tokens, token)) {
                return 0;
            }
            continue;
        }
        
        if (input[i] == '"') {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                Token token;
                token.type = classify_token(buffer);
                strncpy(token.value, buffer, MAX_TOKEN_LEN - 1);
                token.value[MAX_TOKEN_LEN - 1] = '\0';
                
                if (!token_list_add(tokens, token)) {
                    return 0;
                }
                buffer_pos = 0;
            }
            
            size_t j = i;
            buffer_pos = 0;
            while (j < input_len && input[j] != '"') {
                if (buffer_pos >= MAX_TOKEN_LEN - 1) {
                    return 0;
                }
                buffer[buffer_pos] = input[j];
                buffer_pos++;
                j++;
            }
            
            if (j < input_len) {
                if (buffer_pos >= MAX_TOKEN_LEN - 1) {
                    return 0;
                }
                buffer[buffer_pos] = input[j];
                buffer_pos++;
                buffer[buffer_pos] = '\0';
                
                Token token;
                token.type = TOKEN_STRING;
                strncpy(token.value, buffer