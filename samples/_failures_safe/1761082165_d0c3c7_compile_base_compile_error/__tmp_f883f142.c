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

TokenType classify_token(const char* str) {
    if (!str || !str[0]) return TOKEN_EOF;
    
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* token_list) {
    if (!input || !token_list) return 0;
    
    size_t input_len = strlen(input);
    size_t pos = 0;
    
    while (pos < input_len && token_list->count < MAX_TOKENS) {
        while (pos < input_len && isspace(input[pos])) pos++;
        if (pos >= input_len) break;
        
        Token token;
        token.type = classify_token(input + pos);
        size_t token_len = 0;
        
        switch (token.type) {
            case TOKEN_NUMBER:
                while (pos < input_len && isdigit(input[pos]) && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos++];
                }
                break;
                
            case TOKEN_IDENTIFIER:
                while (pos < input_len && (isalnum(input[pos]) || input[pos] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos++];
                }
                break;
                
            case TOKEN_OPERATOR:
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos++];
                }
                break;
                
            case TOKEN_STRING:
                token.value[token_len++] = input[pos++];
                while (pos < input_len && input[pos] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos++];
                }
                if (pos < input_len && input[pos] == '"' && token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[pos++];
                }
                break;
                
            default:
                pos++;
                continue;
        }
        
        if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
            token.value[token_len] = '\0';
            if (!token_list_add(token_list, token)) {
                return 0;
            }
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(token_list, eof_token);
    
    return 1;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(const TokenList* list) {
    if (!list) return;
    
    printf("Tokens found: %zu\n", list->count);
    printf("Type\t\tValue\n");
    printf("----\t