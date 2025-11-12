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
} TokenStack;

TokenStack* create_stack(size_t capacity) {
    if (capacity == 0 || capacity > MAX_TOKENS) return NULL;
    TokenStack* stack = malloc(sizeof(TokenStack));
    if (!stack) return NULL;
    stack->tokens = malloc(capacity * sizeof(Token));
    if (!stack->tokens) {
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->count = 0;
    return stack;
}

void destroy_stack(TokenStack* stack) {
    if (stack) {
        free(stack->tokens);
        free(stack);
    }
}

int push_token(TokenStack* stack, Token token) {
    if (!stack || stack->count >= stack->capacity) return 0;
    if (strlen(token.value) >= MAX_TOKEN_LEN) return 0;
    stack->tokens[stack->count] = token;
    stack->count++;
    return 1;
}

int pop_token(TokenStack* stack, Token* result) {
    if (!stack || !result || stack->count == 0) return 0;
    stack->count--;
    *result = stack->tokens[stack->count];
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    if (isdigit(str[0])) return TOKEN_NUMBER;
    if (is_operator(str[0])) return TOKEN_OPERATOR;
    if (str[0] == '"') return TOKEN_STRING;
    if (isalpha(str[0]) || str[0] == '_') return TOKEN_IDENTIFIER;
    return TOKEN_EOF;
}

int tokenize_string(const char* input, Token** tokens, size_t* token_count) {
    if (!input || !tokens || !token_count) return 0;
    
    size_t capacity = 10;
    *tokens = malloc(capacity * sizeof(Token));
    if (!*tokens) return 0;
    
    size_t count = 0;
    const char* ptr = input;
    
    while (*ptr && count < MAX_TOKENS) {
        while (isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        Token token;
        size_t len = 0;
        TokenType type = classify_token(ptr);
        
        if (type == TOKEN_NUMBER) {
            while (isdigit(ptr[len])) len++;
        } else if (type == TOKEN_IDENTIFIER) {
            while (isalnum(ptr[len]) || ptr[len] == '_') len++;
        } else if (type == TOKEN_OPERATOR) {
            len = 1;
        } else if (type == TOKEN_STRING) {
            len = 1;
            while (ptr[len] && ptr[len] != '"') len++;
            if (ptr[len] == '"') len++;
        } else {
            break;
        }
        
        if (len == 0 || len >= MAX_TOKEN_LEN) break;
        
        strncpy(token.value, ptr, len);
        token.value[len] = '\0';
        token.type = type;
        
        if (count >= capacity) {
            capacity *= 2;
            if (capacity > MAX_TOKENS) capacity = MAX_TOKENS;
            Token* new_tokens = realloc(*tokens, capacity * sizeof(Token));
            if (!new_tokens) {
                free(*tokens);
                return 0;
            }
            *tokens = new_tokens;
        }
        
        (*tokens)[count] = token;
        count++;
        ptr += len;
    }
    
    *token_count = count;
    return 1;
}

void print_tokens(const Token* tokens, size_t count) {
    if (!tokens) return;
    for (size_t i = 0; i < count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
        }
        printf("Token %zu: %s ('%s')\n", i, type_str, tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input,