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
    int line;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
} TokenArray;

TokenArray* create_token_array(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    TokenArray* array = malloc(sizeof(TokenArray));
    if (!array) return NULL;
    
    array->tokens = malloc(sizeof(Token) * initial_capacity);
    if (!array->tokens) {
        free(array);
        return NULL;
    }
    
    array->capacity = initial_capacity;
    array->count = 0;
    return array;
}

void destroy_token_array(TokenArray* array) {
    if (array) {
        free(array->tokens);
        free(array);
    }
}

int token_array_push(TokenArray* array, Token token) {
    if (!array || array->count >= MAX_TOKENS) return 0;
    
    if (array->count >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity > MAX_TOKENS) new_capacity = MAX_TOKENS;
        
        Token* new_tokens = realloc(array->tokens, sizeof(Token) * new_capacity);
        if (!new_tokens) return 0;
        
        array->tokens = new_tokens;
        array->capacity = new_capacity;
    }
    
    array->tokens[array->count] = token;
    array->count++;
    return 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void tokenize(const char* input, TokenArray* tokens) {
    if (!input || !tokens) return;
    
    int line = 1;
    const char* p = input;
    
    while (*p && tokens->count < MAX_TOKENS) {
        while (isspace(*p)) {
            if (*p == '\n') line++;
            p++;
        }
        
        if (!*p) break;
        
        Token token = {0};
        token.line = line;
        
        if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (*p == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            p++;
            while (*p != '"' && *p && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
            if (*p == '"') p++;
        }
        else if (is_operator_char(*p)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else {
            p++;
            continue;
        }
        
        if (!token_array_push(tokens, token)) break;
    }
    
    Token eof_token = {TOKEN_EOF, "", line};
    token_array_push(tokens, eof_token);
}

void print_tokens(const TokenArray* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s (line %d)\n", token->value, token->line);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\" (line %d)\n", token->value, token->line);
                break;
            case TOKEN_EOF:
                printf("EOF (line %d)\n", token->line);
                break;
        }
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter input to tokenize (max %zu chars):\n", sizeof(input) - 1);
    
    if (!fgets(input,