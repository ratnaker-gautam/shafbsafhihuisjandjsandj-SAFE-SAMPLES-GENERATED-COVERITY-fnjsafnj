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
    int column;
} Token;

typedef struct {
    Token* tokens;
    size_t capacity;
    size_t count;
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

void tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    int line = 1;
    int column = 1;
    const char* p = input;
    
    while (*p && tokens->count < MAX_TOKENS) {
        if (isspace(*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
            continue;
        }
        
        Token token = {0};
        token.line = line;
        token.column = column;
        
        if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
                column++;
            }
            token.value[i] = '\0';
        } else if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
                column++;
            }
            token.value[i] = '\0';
        } else if (*p == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            p++;
            column++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
                column++;
            }
            if (*p == '"') {
                p++;
                column++;
            }
            token.value[i] = '\0';
        } else if (is_operator_char(*p)) {
            token.type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
                column++;
            }
            token.value[i] = '\0';
        } else {
            p++;
            column++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) break;
    }
    
    Token eof_token = {TOKEN_EOF, "", line, column};
    token_list_add(tokens, eof_token);
}

void print_tokens(const TokenList* tokens) {
    if (!tokens) return;
    
    for (size_t i = 0; i < tokens->count; i++) {
        const Token* token = &tokens->tokens[i];
        
        switch (token->type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s", token->value);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s", token->value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s", token->value);
                break;
            case TOKEN_STRING:
                printf("STRING: \"%s\"", token->value);
                break;
            case TOKEN_EOF:
                printf("EOF");
                break;
        }
        printf(" (line %d, column %d)\n", token->line, token->column);
    }
}

int main(void) {
    char input[1024];
    
    printf("