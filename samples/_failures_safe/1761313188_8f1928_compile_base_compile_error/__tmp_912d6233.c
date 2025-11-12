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

int tokenize(const char* input, TokenArray* tokens) {
    const char* p = input;
    while (*p != '\0') {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
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
            fprintf(stderr, "Invalid character: %c\n", *p);
            return 0;
        }
        
        token_array_push(tokens, token);
    }
    
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strncpy(eof_token.value, "EOF", MAX_TOKEN_LEN - 1);
    eof_token.value[MAX_TOKEN_LEN - 1] = '\0';
    token_array_push(tokens, eof_token);
    
    return 1;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    TokenArray tokens;
    token_array_init(&tokens, 10);
    
    if (!tokenize(input, &tokens)) {
        token_array_free(&tokens);
        return 1;
    }
    
    printf("Tokens