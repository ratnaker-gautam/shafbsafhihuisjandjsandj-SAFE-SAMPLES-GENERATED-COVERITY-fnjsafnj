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

void tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return;
    
    const char* p = input;
    
    while (*p) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        Token token;
        memset(&token, 0, sizeof(Token));
        
        if (isdigit(*p)) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha(*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (*p == '"') {
            token.type = TOKEN_STRING;
            size_t i = 0;
            p++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
            if (*p == '"') p++;
        }
        else if (is_operator_char(*p)) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else {
            p++;
            continue;
        }
        
        if (!token_list_add(tokens, token)) {
            break;
        }
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strncpy(eof_token.value, "EOF", MAX_TOKEN_LEN - 1);
    eof_token.value[MAX_TOKEN_LEN - 1] = '\0';
    token_list_add(tokens, eof_token);
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
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    TokenList* tokens = token_list_create(10);
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        return