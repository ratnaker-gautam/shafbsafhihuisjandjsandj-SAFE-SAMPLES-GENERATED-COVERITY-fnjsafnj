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
    size_t capacity;
    size_t count;
} TokenArray;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void token_array_init(TokenArray* arr, size_t initial_capacity) {
    arr->tokens = malloc(initial_capacity * sizeof(Token));
    if (arr->tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    arr->capacity = initial_capacity;
    arr->count = 0;
}

void token_array_push(TokenArray* arr, Token token) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity <= arr->capacity) {
            fprintf(stderr, "Capacity overflow\n");
            exit(1);
        }
        Token* new_tokens = realloc(arr->tokens, new_capacity * sizeof(Token));
        if (new_tokens == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
        arr->tokens = new_tokens;
        arr->capacity = new_capacity;
    }
    arr->tokens[arr->count++] = token;
}

void token_array_free(TokenArray* arr) {
    free(arr->tokens);
    arr->tokens = NULL;
    arr->capacity = 0;
    arr->count = 0;
}

TokenArray tokenize(const char* input) {
    TokenArray tokens;
    token_array_init(&tokens, 10);
    
    const char* p = input;
    while (*p != '\0') {
        while (isspace((unsigned char)*p)) {
            p++;
        }
        
        if (*p == '\0') {
            break;
        }
        
        Token token;
        memset(&token, 0, sizeof(Token));
        
        if (isdigit((unsigned char)*p)) {
            token.type = TOKEN_NUMBER;
            size_t i = 0;
            while (isdigit((unsigned char)*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (isalpha((unsigned char)*p) || *p == '_') {
            token.type = TOKEN_IDENTIFIER;
            size_t i = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (is_operator_char(*p)) {
            token.type = TOKEN_OPERATOR;
            size_t i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
        }
        else if (*p == '"') {
            token.type = TOKEN_STRING;
            p++;
            size_t i = 0;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                token.value[i++] = *p++;
            }
            token.value[i] = '\0';
            if (*p == '"') {
                p++;
            }
        }
        else {
            p++;
            continue;
        }
        
        token_array_push(&tokens, token);
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    eof_token.value[0] = '\0';
    token_array_push(&tokens, eof_token);
    
    return tokens;
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

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    TokenArray tokens = tokenize(input);
    
    printf("Tokens:\n");
    for (size_t i = 0; i < tokens.count; i++) {
        printf("Type: %-12s Value: '%s'\