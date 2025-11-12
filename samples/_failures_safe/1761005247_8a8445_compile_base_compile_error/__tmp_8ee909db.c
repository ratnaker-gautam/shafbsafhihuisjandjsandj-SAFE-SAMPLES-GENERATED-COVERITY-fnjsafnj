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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (isalpha(str[0])) return TOKEN_IDENTIFIER;
    if (str[0] == '"') return TOKEN_STRING;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    return TOKEN_IDENTIFIER;
}

int tokenize(const char* input, TokenList* tokens) {
    if (!input || !tokens) return 0;
    
    size_t len = strlen(input);
    size_t i = 0;
    
    while (i < len) {
        while (i < len && isspace(input[i])) i++;
        if (i >= len) break;
        
        Token token = {0};
        size_t token_len = 0;
        
        if (input[i] == '"') {
            token.type = TOKEN_STRING;
            token.value[token_len++] = input[i++];
            
            while (i < len && input[i] != '"' && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            
            if (i < len && input[i] == '"') {
                token.value[token_len++] = input[i++];
            }
        } else if (is_operator(input[i])) {
            token.type = TOKEN_OPERATOR;
            token.value[token_len++] = input[i++];
        } else if (isalnum(input[i])) {
            while (i < len && (isalnum(input[i]) || input[i] == '_') && token_len < MAX_TOKEN_LEN - 1) {
                token.value[token_len++] = input[i++];
            }
            token.type = classify_token(token.value);
        } else {
            i++;
            continue;
        }
        
        token.value[token_len] = '\0';
        if (token_len > 0 && !token_list_add(tokens, token)) {
            return 0;
        }
    }
    
    Token eof_token = {TOKEN_EOF, ""};
    return token_list_add(tokens, eof_token);
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    TokenList* tokens = token_list_create();
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!tokenize(input, tokens)) {
        fprintf(stderr, "Tokenization failed\n");
        token_list_destroy(tokens);
        return 1;
    }
    
    printf("Tokens:\n");
    for (size_t i = 0; i < tokens->count; i++) {
        printf("  %s: '%