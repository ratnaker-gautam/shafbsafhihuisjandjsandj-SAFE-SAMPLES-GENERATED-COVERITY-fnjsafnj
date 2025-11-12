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

TokenType classify_token(const char* str) {
    if (isdigit((unsigned char)str[0])) return TOKEN_NUMBER;
    if (isalpha((unsigned char)str[0])) return TOKEN_IDENTIFIER;
    if (is_operator_char(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, TokenList* list) {
    if (!input || !list) return 0;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len && list->count < MAX_TOKENS) {
        while (i < len && isspace((unsigned char)input[i])) i++;
        if (i >= len) break;
        
        Token token;
        size_t token_start = i;
        size_t token_len = 0;
        
        if (isdigit((unsigned char)input[i])) {
            while (i < len && (isdigit((unsigned char)input[i]) || input[i] == '.')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i];
                }
                i++;
            }
            token.type = TOKEN_NUMBER;
        } else if (isalpha((unsigned char)input[i])) {
            while (i < len && (isalnum((unsigned char)input[i]) || input[i] == '_')) {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i];
                }
                i++;
            }
            token.type = TOKEN_IDENTIFIER;
        } else if (is_operator_char(input[i])) {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i];
            }
            i++;
            token.type = TOKEN_OPERATOR;
        } else if (input[i] == '"') {
            if (token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i];
            }
            i++;
            
            while (i < len && input[i] != '"') {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i];
                }
                i++;
            }
            
            if (i < len && input[i] == '"') {
                if (token_len < MAX_TOKEN_LEN - 1) {
                    token.value[token_len++] = input[i];
                }
                i++;
            }
            token.type = TOKEN_STRING;
        } else {
            i++;
            continue;
        }
        
        token.value[token_len] = '\0';
        if (!token_list_add(list, token)) return 0;
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    token_list_add(list, eof_token);
    
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

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!f