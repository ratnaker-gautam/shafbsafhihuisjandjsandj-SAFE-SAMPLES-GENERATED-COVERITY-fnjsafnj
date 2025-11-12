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

int token_list_grow(TokenList* list) {
    if (!list) return 0;
    
    size_t new_capacity = list->capacity * 2;
    if (new_capacity <= list->capacity) return 0;
    
    Token* new_tokens = realloc(list->tokens, sizeof(Token) * new_capacity);
    if (!new_tokens) return 0;
    
    list->tokens = new_tokens;
    list->capacity = new_capacity;
    return 1;
}

int token_list_add(TokenList* list, Token token) {
    if (!list) return 0;
    
    if (list->count >= list->capacity) {
        if (!token_list_grow(list)) return 0;
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
    
    int line = 1;
    const char* p = input;
    
    while (*p) {
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
            while (*p && *p != '"' && i < MAX_TOKEN_LEN - 1) {
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
        
        if (!token_list_add(tokens, token)) {
            break;
        }
    }
    
    Token eof_token = {TOKEN_EOF, "", line};
    token_list_add(tokens, eof_token);
}

const char* token_type_name(TokenType type) {
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
    
    TokenList* tokens = token_list_create(10);
    if (!tokens) {
        fprintf(stderr, "Failed to create token list\n");
        return 1;
    }
    
    tokenize(input,